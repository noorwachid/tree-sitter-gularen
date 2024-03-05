(comment) @comment

(chapter) @markup.heading.1
(section) @markup.heading.2
(subsection) @markup.heading.3

(bullet) @keyword
(index) @number
(v) @diff.plus
(x) @diff.minus

(bold) @markup.strong
(italic) @markup.italic
(monospaced) @string

(line_break) @diff.minus
(page_break) @diff.minus
(dinkus) @keyword

(code_inline) @markup.raw

(code_block (code_content) @markup.raw.block)

(code_lang) @punctuation
(fence_open) @punctuation
(fence_close) @punctuation

(label) @markup.link.label
(admon_label) @function
(datetime_content) @string.escape

(exclamation) @punctuation
(question) @punctuation
(caret) @punctuation
(equal) @punctuation

(angle_open) @punctuation
(angle_close) @punctuation
(square_open) @punctuation
(square_close) @punctuation
(curly_open) @punctuation
(curly_close) @punctuation
(paren_open) @punctuation
(paren_close) @punctuation

(footnote_desc (resource) @function)
(resource) @markup.link.url
