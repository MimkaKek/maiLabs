(defun get-half-summ (x y z)
    (let ( (mid   (if (and (> x y) (< x z)) x (if (and (> y x) (< y z) ) y z) ) ) 
           (high  (if (and (> x y) (> x z)) x (if (and (> y x) (> y z) ) y z) ) )
         )
         (/ (+ mid high) 2)
    )
)

(defun getXY-half-summ (x y z)
    (if (< x y) (/ (+ z y) 2) (/ (+ z x) 2))
)
(defun convert-three (x y z)
    (let ( (sum (+ x y z)) 
           (half-summ (get-half-summ x y z))
           (halfXY-summ (getXY-half-summ x y z))
         )
         (if (< sum 1.0)
             (if (and (< x y) (< x z))
                 (values half-summ y z)
                 (if (and (< y z) (< y z))
                     (values x half-summ z)
                     (values x y half-summ)
                 )
             )
             
             (if (< x y) (values halfXY-summ y z) (values x halfXY-summ z))
         )
    )
)

(defun three-print (x y z)
    (multiple-value-bind (a b c) (convert-three x y z)
        (print a)
        (print b)
        (print c)
    )
)
(print "Пример 1:")
(three-print 4.0 2.0 1.0)
(print "Пример 2:")
(three-print 0.2 0.1 0.3)