(set-logic QF_LIA)
(declare-fun x () Int)
(assert (< (abs x) x))
(check-sat)

