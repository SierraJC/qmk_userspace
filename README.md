# QMK Userspace

This userspace is configured to build against [vial-kb/vial-qmk](https://github.com/vial-kb/vial-qmk) on branch `vial`. The firmware source is tracked in this repository as the `qmk_firmware` git submodule.

## Howto configure your build targets

1. Fork this repository
1. Clone your fork with submodules: `git clone --recurse-submodules <your-fork-url>`
1. `cd` into this repository's clone directory
1. Run `mise install`
    - This installs tools and runs `mise run init`.
    - `mise run init` initialises submodules, sets `qmk config user.qmk_home` to `./qmk_firmware`, and sets `qmk config user.overlay_dir` to this userspace.
1. Add a new keymap for your board using `qmk new-keymap`
    - This creates a new keymap in the `keyboards` directory in the same location used in the main QMK repository. For example, `keyboards/planck/keymaps/<your keymap name>`.
    - You can also create a keymap with `qmk new-keymap -kb <your_keyboard> -km <your_keymap>`.
    - Alternatively, add your keymap manually in that location.
    - `layouts/<layout name>/<your keymap name>/keymap.*` is also supported.
1. Add your keymap(s) to the build by running `qmk userspace-add -kb <your_keyboard> -km <your_keymap>`
    - This automatically updates `qmk.json`.
    - `qmk userspace-remove -kb <your_keyboard> -km <your_keymap>` removes a target.
    - `qmk userspace-list` lists current build targets.
1. Commit your changes

## Howto build with GitHub

1. In the GitHub Actions tab, enable workflows
1. Push your changes above to your forked GitHub repository
1. Look at the GitHub Actions for a new workflow run
1. Wait for the workflow to complete
1. Inspect the Releases tab for the latest firmware build

## Howto build locally

1. Ensure initialisation is complete with `mise install` (or rerun `mise run init`)
1. Build the default firmware target: `mise run build`
1. Build and generate `compile_commands.json`: `mise run compile`
1. Build and flash: `mise run flash`

Alternatively, if you configured your build targets in `qmk.json`, run `qmk userspace-compile`.

## Managing qmk_firmware

1. If you cloned without `--recurse-submodules`, run `git submodule update --init --recursive`
1. To update to the latest `vial` branch revision of `vial-kb/vial-qmk`, run `mise run update-firmware`
1. Commit the updated `qmk_firmware` submodule revision in this userspace repository

## Extra info

If you want GitHub Actions to build against a different repository or branch when the `qmk_firmware` submodule is not present, update `.github/workflows/build_binaries.yaml`.
