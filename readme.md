# Tree Sitter for Gularen Markup Language

Tree sitter implementation for [Gularen markup language](https://github.com/noorwachid/gularen/)

## How To Install
### Neovim
1. Install [nvim-treesitter](https://github.com/nvim-treesitter/nvim-treesitter/) plugin
2. Add this code to your `init.lua` 
  ```lua
  local parser_config = require "nvim-treesitter.parsers".get_parser_configs()
  parser_config.gularen = {
    install_info = {
      url = "~/dev/projects/personal/tree-sitter-gularen",
      files = {"src/parser.c", "src/scanner.c"},
      generate_requires_npm = false,
      requires_generate_from_grammar = false,
    },
    filetype = "gularen",
  }
  ```
3. Reload your `init.lua` with `:so ~/.config/nvim/init.lua`
4. Install the parser with `:TSInstall gularen`
5. Copy `queries/neovim/*.scm` to `~/.config/nvim/queries/gularen/`
6. Reload your neovim by closing and opening it again

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
- [ ] 10 blockquote
- [x] 11 admonition
- [x] 11 datetime
