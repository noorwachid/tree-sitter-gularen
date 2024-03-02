(comment) @comment

(chapter) @markup.heading.1
(section) @markup.heading.2
(subsection) @markup.heading.3

(bold) @markup.strong
(italic) @markup.italic
(monospaced) @markup.monospaced

(code_inline) @markup.raw
(code_inline (curly_open) @markup.raw.operator)
(code_inline (curly_close) @markup.raw.operator)

(code_block (code_content) @markup.raw.block)
(code_block (fence_open) @punctuation)
(code_block (fence_close) @punctuation)
