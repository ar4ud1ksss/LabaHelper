set terminal png
set output "graph.png"
set xlabel "I, A"
set ylabel "U, V" 
plot "grafik.csv" using 2:1 with lines notitle

