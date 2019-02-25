; letset
"#1"
(define a 7)
(display "a=")(display a)(newline)
;_____________
"#2"
(define(f x)
  (let(( a 5))
      (+ x a)
  )
)
(display"(f 1)=")(display(f 1))(newline)
(display "a=")(display a)(newline)
;_______________
"#3"
(define(sa2)
 (set! a 2)
 a)
(display"(sa2)=")(display(sa2))(newline)
(display "a=")(display a)(newline)
;_____________
"#4"
(define(g x a)
  (set! a 5)
  (+ x a))
(display"(g 1 0)=")(display(g 1 0))(newline)
(display "a=")(display a)(newline)
;_______________
