module.exports = grammar({
	name: 'gularen',

	rules: {
		document: $ => repeat($._block),

		_block: $ => choice(
			$.chapter,
			$.section,
			$.subsection,
			$.subtitle,

			$.code_block_lang,
			$.code_block,

			$.list,
			$.numbered_list,
			$.todo_list,

			$.blockquote,

			$.paragraph,

			$._newline_plus,
		),

		chapter: $ => seq($.head3, repeat1($._inline), $._end_block),
		section: $ => seq($.head2, repeat1($._inline), $._end_block),
		subsection: $ => seq($.head1, repeat1($._inline), $._end_block),
		subtitle: $ => seq($.head0, repeat1($._inline), $._end_block),

		list: $ => prec.right(repeat1(seq($.bullet, repeat1($._inline), $._end_block))),

		numbered_list: $ => prec.right(repeat1(seq($.index, repeat1($._inline), $._end_block))),

		todo_list: $ => prec.right(repeat1(seq($.square_open, choice($.blank, $.v, $.x), $.square_close, repeat1($._inline), $._end_block))),

		blockquote: $ => prec.right(repeat1(seq($.slash, repeat1($._inline), $._end_block))),

		code_block_lang: $ => seq($.fence_open, $.code_lang, $._newline, $.code_content, $.fence_close),
		code_block: $ => seq($.fence_open, $._newline, $.code_content, $.fence_close),

		code_content: $ => repeat1($._code_line),

		paragraph: $ => prec.right(seq(repeat1(choice($._inline, $._newline)), optional($._newline_plus))),

		_end_block: $ => choice($._newline, $._newline_plus),

		_inline: $ => choice(
			$.comment, 

			$.dinkus,

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

			$.page_break,
			$.line_break,

			$.admon,
			$.datetime,

			$.account_tag,
			$.hash_tag,

			$.text, 
		),

		comment: $ => /~.*/,

		dinkus: $ => '***',

		page_break: $ => '<<<',
		line_break: $ => '<<',
		admon: $ => seq($.angle_open, $.admon_label, $.angle_close),
		datetime: $ => seq($.angle_open, $.datetime_content, $.angle_close),

		angle_open: $ => '<',
		angle_close: $ => '>',

		admon_label: $ => /[A-Za-z][^>]+/,
		datetime_content: $ => /[0-9- :]+/,

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

		$.head3,
		$.head2,
		$.head1,
		$.head0,

		$.bullet,
		$.index,
		$.blank,
		$.v,
		$.x,

		$.slash,

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

		$.account_tag,
		$.hash_tag,

		$.text,
	],

});

