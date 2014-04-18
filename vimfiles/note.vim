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
syn keyword 	snlType		kpt topic
syn keyword	snlProperty	what when how
syn keyword	snlConnect	of compares ref

syn match	snlNum		'\d\*\.\d\+'
syn match	snlNum		'[-+]\d\*\.\d\+'
syn keyword	snlTodo		contained TODO FIXME XXX NOTE

" This is a descrpition of Comment
syn match	snlComment	"\v#.*$"	contains=snlTodo
syn match	snlComment	"\v(\*.*\*)" 	contains=snlTodo

syn region	snlBlock	start='{' end='}' fold transparent

" This is a description of String, copyed from python syntax
syn region  	snlStr
      \ start=+[uU]\=\z(['"]\)+ end="\z1" skip="\\\\\|\\\z1"
      \ contains=snlEscape
syn region  	snlRawStr
      \ start=+[uU]\=[rR]\z(['"]\)+ end="\z1" skip="\\\\\|\\\z1"
      \ contains=@Spell

syn match	snlEscape	+\\[abfnrtv'"\\]+ contained

let b:current_syntax='snl'
hi def link	snlTodo		Todo
hi def link	snlComment	Comment
hi def link	snlType		Type
hi def link	snlNum		Number
hi def link	snlStr		String
hi def link	snlRawStr	String
hi def link	snlConnect	Label
hi def link	snlProperty	Label
hi def link	snlEscape	Special

