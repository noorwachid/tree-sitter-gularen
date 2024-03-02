module.exports = grammar({
	name: 'gularen',

	rules: {
		document: $ => repeat($._block),

		_block: $ => choice(
			$.chapter,
			$.section,
			$.subsection,

			$.code_block_lang,
			$.code_block,

			$.paragraph,
		),

		indent: $ => seq($.indent_open, repeat1($._block), $.indent_close),

		chapter: $ => seq($.head3, repeat1($._inline), $._end_block),
		section: $ => seq($.head2, repeat1($._inline), $._end_block),
		subsection: $ => seq($.head1, repeat1($._inline), $._end_block),

		code_block_lang: $ => seq($.fence_open, $.code_lang, $._newline, $.code_content, $.fence_close),
		code_block: $ => seq($.fence_open, $._newline, $.code_content, $.fence_close),

		code_content: $ => repeat1($._code_line),

		paragraph: $ => prec.right(seq(repeat1(choice($._inline, $._newline)), optional($._newline_plus))),

		_end_block: $ => choice($._newline, $._newline_plus),

		_inline: $ => choice(
			$.comment, 
			$.bold,
			$.italic,
			$.monospaced,
			$.code_inline_lang,
			$.code_inline,

			$.footnote_ref,
			$.footnote_desc,

			$.include,

			$.view_label,
			$.view,

			$.link_label,
			$.link,

			$.text, 
		),

		comment: $ => /~.*/,

		bold: $ => seq('*', $._inline, '*'),
		italic: $ => seq('_', $._inline, '_'),
		monospaced: $ => seq('`', $._inline, '`'),

		view_label: $ => seq($.exclamation, $.square_open, $.resource, $.square_close, $.paren_open, $.label, $.paren_close),
		view: $ => seq($.exclamation, $.square_open, $.resource, $.square_close),

		link_label: $ => seq($.square_open, $.resource, $.square_close, $.paren_open, $.label, $.paren_close),
		link: $ => seq($.square_open, $.resource, $.square_close),

		footnote_ref: $ => seq($.caret, $.square_open, $.resource, $.square_close),
		footnote_desc: $ => seq($.equal, $.square_open, $.resource, $.square_close),

		include: $ => seq($.question, $.square_open, $.resource, $.square_close),

		code_inline_lang: $ => seq($.curly_open, $.code_inline_content, $.curly_close, $.paren_open, $.label, $.paren_close),
		code_inline: $ => seq($.curly_open, $.code_inline_content, $.curly_close),
	},

	externals: $ => [
		$._newline,
		$._newline_plus,

		$._indent_open,
		$._indent_close,

		$.head3,
		$.head2,
		$.head1,

		$.fence_open,
		$.fence_close,
		$.code_lang,
		$._code_line,

		$.curly_open,
		$.curly_close,
		$.code_inline_content,

		$.exclamation,
		$.question,
		$.caret,
		$.equal,

		$.square_open,
		$.square_close,
		$.resource,

		$.paren_open,
		$.paren_close,
		$.label,

		$.text,
	],

});

