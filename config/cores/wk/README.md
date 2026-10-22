# ACT Configuration for the WK RV64GC

## Overview

The `wk-rv64gc` configuration targets the WK RV64GC core with the Sail RISC-V reference
model. It carries a small set of **local patches** that work around toolchain and
reference-model limitations without touching any upstream file in `tests/`,
`framework/`, or `config/cores/wk/wk-rv64gc/` itself.

All patches live under `config/cores/wk/wk-rv64gc/patches/` and are applied at runtime by
`sitecustomize.py`, which the build picks up via `PYTHONPATH`.

| Config      | ISA    | Modes            | Ref model |
| ----------- | ------ | ---------------- | --------- |
| `wk-rv64gc` | RV64GC | M + S + U, Sv39  | Sail 0.15 |

## Why local patches exist

Three upstream behaviours are not suitable for this DUT, and each is compensated by a
small, narrowly-scoped patch:

1. **MMIO addresses beyond `la` reach.** `RVMODEL_MTIME_ADDRESS`,
   `RVMODEL_MTIMECMP_ADDRESS`, and `RVMODEL_MSIP_ADDRESS` are above 1 GB, which
   `la` cannot encode under the medlow code model. Any `LA(reg, <that address>)`
   emitted by the test generator is rewritten to `LI(reg, <that address>)`.

2. **`RVTEST_CODE_END` tail hook.** In the Sail reference build, the code path after
   `RVTEST_TSBI_GOTO_MMODE` must not jump to `rvmodel_halt_pass` before the epilogs run.
   A single `RVTEST_POST_MMODE_HOOK` line is injected into `rvtest_setup.h` right after
   `RVTEST_TSBI_GOTO_MMODE`, and is defined to `nop` under `_SAIL_MACROS_H` and to
   `j rvmodel_halt_pass` otherwise.

3. **Fast failure in RTL simulation.** When a test fails, the full diagnostic reporter
   costs several thousand cycles. Every `failedtest_*` entry point in
   `rvtest_failure_code.h` is prefixed with `j rvmodel_halt_fail` so RTL simulation
   ends immediately. The Sail signature flow is unaffected because it does not enter
   these entry points.

Additionally, two build-quality conveniences are provided:

- **Reference model step limit.** `--inst-limit 2000000` is inserted immediately after
  `sail_riscv_sim` in every reference-model invocation to bound runaway tests.
- **Automatic packaging.** After the build finishes, every `work/<config>/` directory is
  scanned; a `lib/` directory is assembled from the environment headers, the patches
  directory, `rvmodel_macros.h`, `link.ld`, and the generated `rvtest_config.h`, and
  `cases/` + `lib/` are packed into `work/<config>/<config>.tar.gz`.

## Directory layout

```text
config/cores/wk/wk-rv64gc/
├── link.ld
├── rvmodel_macros.h
├── sail.json
├── test_config.yaml
├── wk-rv64gc.yaml
└── patches/
    ├── patches.py                 # patch registry + atexit packaging
    ├── sitecustomize.py           # Python startup hook, imports patches
    ├── gen_rvtest_header.py       # derives override headers from tests/env/
    └── override_header/           # generated; do not edit
        ├── riscv_arch_test.h
        ├── rvtest_setup.h
        └── rvtest_failure_code.h
```

`patches.py` is the single source of truth for all local modifications. Everything under
`override_header/` is regenerated from `tests/env/` on every run and must not be edited
by hand.

## Building and running

```bash
PYTHONPATH=config/cores/wk/wk-rv64gc/patches:$PYTHONPATH \
CONFIG_FILES=config/cores/wk/wk-rv64gc/test_config.yaml \
make --jobs 12 EXTENSIONS=Sm,I DEBUG=true
```

After the build completes:

```text
work/wk-rv64gc/
├── cases/<group>/<test>.{cfg,S,results}
├── lib/*.h, link.ld, rvtest_config.h
└── wk-rv64gc.tar.gz                # contains cases/ and lib/
```

## Patch details

### `patches.py`

Registers all modifications against upstream modules via a declarative `@patch(...)`
decorator. Two things deserve attention:

- `_once` uses a **per-function** idempotency flag
  (`mod._user_patched_<fn_name>`). Sharing a single `_user_patched` across all patches
  on the same module would silently skip every patch after the first one, because
  `_Patcher` applies the module's patches in registration order.

- The `_Patcher` MetaPathFinder must be inserted **before any `from act... import`** in
  this file. Importing `act.build_types` first would trigger `act.build_plan` before
  the finder is on `sys.meta_path`, and `generate_build_plan` would never be wrapped.

### `gen_rvtest_header.py`

Reads the four upstream headers from `tests/env/` and writes derived copies to
`override_header/`:

| Source                   | Transformation                                                        |
| ------------------------ | --------------------------------------------------------------------- |
| `riscv_arch_test.h`      | verbatim copy (entry-point for `-I` shadowing)                        |
| `rvtest_setup.h`         | `RVTEST_POST_MMODE_HOOK` injected after `RVTEST_TSBI_GOTO_MMODE`      |
| `test_setup.h`           | `LA(reg, RVMODEL_MTIME_*\|MSIP)` rewritten to `LI(reg, ...)`          |
| `rvtest_failure_code.h`  | `j rvmodel_halt_fail` inserted at every `failedtest_*` entry point    |

The transformation is idempotent: running it twice produces the same file, and a
pre-existing `RVTEST_POST_MMODE_HOOK` or `j rvmodel_halt_fail` is not duplicated.

### `sitecustomize.py`

Loaded automatically by Python at startup because `patches/` is on `PYTHONPATH`.
It adds the ACT4 root to `sys.path`, ensures `override_header/` is fresh by calling
`gen_rvtest_header.main()`, then `import patches` to register the modifications.

`ACT4_REPO_ROOT` should point at the ACT4 checkout; if unset, the current working
directory is used.

## Overriding `-I` order

The patches directory is prepended to the compiler's include search path by patching
`Toolchain.compile_prefix`:

```python
[base[0], f"-I{OVERRIDES_DIR}", *base[1:], "-mno-relax"]
```

`OVERRIDES_DIR` is `config/cores/wk/wk-rv64gc/patches/override_header/`. Because
`-I` is searched before the compiler's built-in paths, the derived
`riscv_arch_test.h` is picked up first, and its `#include "rvtest_setup.h"` etc.
resolve to the sibling files under `override_header/` rather than to `tests/env/`.

## Syncing with upstream

Because no upstream file is modified, a `git pull` on ACT4 never conflicts with these
patches. After every pull, re-run any build; `gen_rvtest_header.py` re-derives the
override headers from the latest `tests/env/` on the first invocation.

If upstream changes the shape of an override header (for example, renames a label that
`gen_rvtest_header.py` relies on), the corresponding transformation will fail loudly
and must be updated in `gen_rvtest_header.py`. The `patches.py` file has no dependency
on upstream text and rarely needs changes; only the `@patch(...)` target names might
need to move if upstream renames a module or a function.

## Known limitations

- `--inst-limit` is a Sail-only flag; the SPIKE path is not affected because the
  patch checks `ref_model_type` indirectly via the shape of the returned command
  (a list whose first element is the executable).
- The automatic packaging runs in an `atexit` handler, so it fires once per Python
  process. If a build is interrupted before Python exits, the archive is not produced.
- `lib/` is rebuilt on each run; any manual edits to files under `lib/` are lost.

## Platform

- RAM and MMIO layout are described in `sail.json`. The CLINT base used by the Sail
  reference model is defined via `SAIL_CLINT_BASE_ADDRESS` and
  `SAIL_SIMPLE_INTERRUPT_GENERATOR_BASE_ADDRESS`, both emitted as compiler defines
  by `act.build_plan._sail_platform_defines`.
- `RVMODEL_MTIME_ADDRESS`, `RVMODEL_MTIMECMP_ADDRESS`, and `RVMODEL_MSIP_ADDRESS` point
  into the CLINT region and exceed the reach of `la`, which is the reason for the
  `LA → LI` rewrite described above.
