# Tree Sitter for Gularen Markup Language

Tree sitter implementation for [Gularen markup language](https://github.com/noorwachid/gularen/)

<img width="1426" alt="Screenshot 2024-03-05 at 17 26 30" src="https://github.com/noorwachid/tree-sitter-gularen/assets/42460975/723c6991-6048-4d06-b9f1-f2d6ba8f3b82">

## How To Install
### Neovim
1. Install [nvim-treesitter](https://github.com/nvim-treesitter/nvim-treesitter/) plugin
2. Clone this repo
3. Add this code to your `init.lua` 
  ```lua
  local parser_config = require "nvim-treesitter.parsers".get_parser_configs()
  parser_config.gularen = {
    install_info = {
      url = "~/location-of-cloned-repo/tree-sitter-gularen",
      files = {"src/parser.c", "src/scanner.c"},
      generate_requires_npm = false,
      requires_generate_from_grammar = false,
    },
    filetype = "gularen",
  }
  ```
4. Reload your `init.lua` with `:so ~/.config/nvim/init.lua`
5. Install the parser with `:TSInstall gularen`
6. Copy `queries/neovim/*.scm` to `~/.config/nvim/queries/gularen/`
7. Reload your neovim by closing and opening it again

Someone should add this parser to [nvim-treesitter](https://github.com/nvim-treesitter/nvim-treesitter/) because I'm not doing it.

Todo list:
- [x] 01 comment
- [x] 02 style
- [x] 03 heading
- [x] 04 paragraph
- [x] 05 indentation
- [x] 06 break
- [x] 07 list
- [ ] 08 table
- [x] 09 code
- [x] 0A linker
- [x] 0B viewer
- [x] 0C inclusion
- [x] 0D footnote
- [ ] 0E punctuation
- [ ] 0F emoji
- [x] 10 blockquote
- [x] 11 admonition
- [x] 11 datetime
