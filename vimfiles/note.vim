" Vim syntax file
" Language:	SNL
" Maintainer:	Xeechem Chou <xeechou@gmail.com>
" Last Change:	2014 Apr 18

" Remember rename the syntax file when you use it
" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
	finish
endif

" Basic Keywords
<<<<<<< HEAD
syn keyword 	snlType		kpt topic proc
syn keyword	snlProperty	what when how why relation
syn keyword	snlConnect	of compares ref AKA
syn keyword	snlStatement	has which
=======
syn keyword 	snlType		kpt topic proc func
syn keyword	snlProperty	what when how where who why
syn keyword	snlConnect	of extern ref AKA return
syn keyword	snlStatement	case of esac let in tel
syn keyword	snlStatement	connect with to from about
>>>>>>> parser

syn match	snlNum		'\d\*\.\d\+'
syn match	snlNum		'[-+]\d\*\.\d\+'
syn keyword	snlTodo		contained TODO FIXME XXX NOTE

" This is a descrpition of Comment
syn match	snlComment	"\v#.*$"	contains=snlTodo
" This is the trick about, contains yourself in, then you can write nested rule
syn region	snlnComment	start="(\*" end="\*)"	contains=snlTodo,snlnComment

syn region	snlBlock	start='{' end='}' fold transparent

" This is a description of String, copyed from python syntax
syn region  	snlStr
      \ start=+[uU]\=\z(['"]\)+ end="\z1" skip="\\\\\|\\\z1"
      \ contains=snlEscape
syn region  	snlRawStr
      \ start=+[uU]\=[rR]\z(['"]\)+ end="\z1" skip="\\\\\|\\\z1"
      \ contains=@Spell

syn match	snlEscape	+\\[abfnrtv'"\\\$]+ contained

let b:current_syntax='snl'
hi def link	snlTodo		Todo
hi def link	snlComment	Comment
hi def link	snlnComment	Comment
hi def link	snlType		Type
hi def link	snlNum		Number
hi def link	snlStr		String
hi def link	snlRawStr	String
hi def link	snlConnect	Statement
hi def link	snlStatement	Statement
hi def link	snlProperty	function
hi def link	snlEscape	Special

