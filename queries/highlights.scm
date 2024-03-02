(comment) @comment

(chapter) @markup.heading.1
(section) @markup.heading.2
(subsection) @markup.heading.3

(bold) @markup.strong
(italic) @markup.italic
(monospaced) @string

(code_inline) @markup.raw
(code_inline (curly_open) @markup.raw.operator)
(code_inline (curly_close) @markup.raw.operator)

(code_inline_lang (label) @string)
(code_inline_lang (curly_open) @punctuation)
(code_inline_lang (curly_close) @punctuation)
(code_inline_lang (paren_open) @punctuation)
(code_inline_lang (paren_close) @punctuation)

(code_block (code_content) @markup.raw.block)
(code_block (fence_open) @punctuation)
(code_block (fence_close) @punctuation)

(code_block_lang (code_lang) @punctuation)
(code_block_lang (fence_open) @punctuation)
(code_block_lang (fence_close) @punctuation)
