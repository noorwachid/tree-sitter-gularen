module.exports = grammar({
	name: 'gularen',

	rules: {
		document: $ => repeat($._block),

		_block: $ => choice(
			$.chapter,
			$.section,
			$.subsection,
			$.subtitle,

			$.code_block_labeled,
			$.code_block,

			$.list,
			$.numbered_list,
			$.check_list,

			$.paragraph,

			$._newline_plus,
		),

		chapter: $ => seq($.head3, repeat1($._inline), $._end_block),
		section: $ => seq($.head2, repeat1($._inline), $._end_block),
		subsection: $ => seq($.head1, repeat1($._inline), $._end_block),
		subtitle: $ => seq($.head0, repeat1($._inline), $._end_block),

		list: $ => prec.right(repeat1(seq($.bullet, repeat1($._inline), $._end_block))),

		numbered_list: $ => prec.right(repeat1(seq($.index, repeat1($._inline), $._end_block))),

		check_list: $ => prec.right(repeat1(seq($.square_open, choice($.blank, $.x), $.square_close, repeat1($._inline), $._end_block))),

		code_block_labeled: $ => seq($.fence_open, $.code_block_label, $._newline, $.code_block_content, $.fence_close),
		code_block: $ => seq($.fence_open, $._newline, $.code_block_content, $.fence_close),
		code_block_content: $ => repeat1($._code_block_line),

		paragraph: $ => prec.right(seq(repeat1(choice($._inline, $._newline)), optional($._newline_plus))),

		_end_block: $ => choice($._newline, $._newline_plus),

		_inline: $ => choice(
			$.comment, 
			$.annotation,
			$.reference,

			$.dinkus,

			$.bold,
			$.italic,
			$.underline,
			$.highlight,
			$.code_inline,

			$.in_text,
			$.footnote,

			$.include,

			$.view_labeled,
			$.view,

			$.link_labeled,
			$.link,

			$.break,

			$.admon,

			$.datetime,
			$.account_tag,
			$.hash_tag,

			$.coloncolon,

			$.escape,

			$.text, 
		),

		comment: $ => seq(/~[^~].*/, $._end_block),
		annotation: $ => seq('~~', $.annotation_key, $.annotation_assign, $.annotation_value, $._end_block),
		annotation_key: $ => /[0-9A-Za-z-]+/,
		annotation_assign: $ => / += +/,
		annotation_value: $ => /.*/,

		reference: $ => seq('~~ &', $.reference_key),
		reference_key: $ => /.*/,

		coloncolon: $ => '::',
		dinkus: $ => '***',

		break: $ => /<{1,3}/,
		admon: $ => seq($.admon_marker, $.admon_label, $.admon_marker),
		admon_marker: $ => '//',
		admon_label: $ => /[A-Za-z][^>]+/,

		datetime: $ => /\+[0-9- :]+/,

		bold: $ => seq('*', repeat1(choice($.text, $.italic, $.underline, $.highlight)), '*'),
		italic: $ => seq('/', repeat1(choice($.text, $.bold, $.underline, $.highlight)), '/'),
		underline: $ => seq('_', repeat1(choice($.text, $.bold, $.italic, $.highlight)), '_'),
		highlight: $ => seq('==', repeat1(choice($.text, $.bold, $.italic, $.underline)), '=='),
		code_inline: $ => seq($.backtick, $.code_inline_content, $.backtick),

		view_labeled: $ => seq($.exclamation, $.square_open, $.resource, $.square_close, $.paren_open, $.label, $.paren_close),
		view: $ => seq($.exclamation, $.square_open, $.resource, $.square_close),

		link_labeled: $ => seq($.square_open, $.resource, $.square_close, $.paren_open, $.label, $.paren_close),
		link: $ => seq($.square_open, $.resource, $.square_close),

		footnote: $ => seq($.caret, $.square_open, $.resource, $.square_close),
		in_text: $ => seq($.ampersand, $.square_open, $.resource, $.square_close),
		include: $ => seq($.question, $.square_open, $.resource, $.square_close),

		escape: $ => /\\./,
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
		$.x,

		$.fence_open,
		$.fence_close,
		$.code_block_label,
		$._code_block_line,

		$.backtick,
		$.code_inline_content,

		$.exclamation,
		$.question,
		$.caret,
		$.ampersand,

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

