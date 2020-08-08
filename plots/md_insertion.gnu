load "common.gnu"

set title "Insertion sort with various medium sized sets."

plot 	"../data/md/insertion_inverted" t "inverted" with lines lw 2,\
	"../data/md/insertion_random" t "random" with lines lw 2,\
	"../data/md/insertion_sorted" t "sorted" with lines lw 2