module.exports = grammar({
	name: 'gularen',

	rules: {
		document: $ => repeat($._block),

		_block: $ => choice(
			$.chapter,
			$.section,
			$.subsection,

			$.code_block,

			$.paragraph,
		),

		chapter: $ => seq($.head3, repeat1($._inline), choice($._newline, $._newline_plus)),
		section: $ => seq($.head2, repeat1($._inline), choice($._newline, $._newline_plus)),
		subsection: $ => seq($.head1, repeat1($._inline), choice($._newline, $._newline_plus)),

		code_block: $ => seq($.fence_open, $.raw, $.fence_close),

		paragraph: $ => prec.right(seq(repeat1(choice($._inline, $._newline)), optional($._newline_plus))),

		_inline: $ => choice(
			$.comment, 
			$.bold,
			$.italic,
			$.monospaced,
			$.text, 
		),

		comment: $ => /~.*/,

		bold: $ => seq('*', $._inline, '*'),
		italic: $ => seq('_', $._inline, '_'),
		monospaced: $ => seq('`', $._inline, '`'),
	},

	externals: $ => [
		$._end,
		$._newline,
		$._newline_plus,
		$.head3,
		$.head2,
		$.head1,
		$.fence_open,
		$.fence_close,
		$.raw,
		$.text,
	],

});

