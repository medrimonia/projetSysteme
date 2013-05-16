rapport_16mai.pdf: rapport_16mai.tex
	pdflatex $< && pdflatex $<

rapport_int.pdf: rapport_int.tex
	pdflatex rapport_int.tex
	pdflatex rapport_int.tex


clean:
	rm *.log
	rm *.aux
	rm *.pdf