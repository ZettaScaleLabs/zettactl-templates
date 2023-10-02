<div align="center">

  <h1>🌾✨ zettactl-templates</h1>

  <p>
    <strong> Template files for zettactl </strong>
  </p>

  <p>
    <a href="https://choosealicense.com/licenses/epl-2.0/"><img alt="License EPL" src="https://img.shields.io/badge/License-EPL%202.0-blue"/></a>
    <a href="https://opensource.org/licenses/Apache-2.0"><img alt="License EPL" src="https://img.shields.io/badge/License-Apache%202.0-blue.svg"/></a>
  </p>

<sub>Built by the <a href="https://zenoh.io">Zenoh</a> team at <a href="https://www.zettascale.tech">ZettaScale</a></sub>
</div>

## About
This repository contains the template files for the create command from zettactl CLI. Currently we support the following:

<div align="center">

| Templates     | RUST          | C                 | CPP               | PYTHON    |
| :-------:     | :-----------: | :---------------: | :---------------: | :-------: |
| Subscriber    | ✅            | ✅                 | ✅                | ✅        |
| Publisher     | ✅            | ✅                 | ✅                | ✅        |
| Query         | ✅            | ✅                 | ✅                | ✅        |
| Get           | ✅            | ✅                 | ✅                | ✅        |
| Common files  | ```Cargo.toml```    | ```CMakeLists.txt```    | ```CMakeLists.txt```    | -         |

</div>

## Install
This repository is usually cloned into the ```${HOME}/.zettactl/templates/``` directory when the ```zettactl``` CLI tool is installed, so there is no need to manually install this folder.

## Usage
The ```zettactl``` CLI tool will look for these template files locally each time the ```create``` command is used and all this happens in the background. However, the user can decide to change the CLI generated files as he or she see fits by editing the templates.

## Want to contribute?
Check these ideas and feel free to create PRs:

* Automatically track the latest examples for each binding
* Add #ifdev for c and cpp templates for zenoh-pico vs zenoh-c
* Comment w/ more info, advices or suggestions (like link to online docs)
* Add unit and integration tests for each template and language
* Create a simple CI