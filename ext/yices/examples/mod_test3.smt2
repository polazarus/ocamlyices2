(set-logic QF_LIA)
(declare-fun x () Int)
(assert (< x (- 4)))
(assert (= (mod x (- 5)) 2))
(check-sat)
(get-model)
(get-value (x (mod x (- 5)) (div x (- 5))))
