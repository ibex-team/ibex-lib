SVG=$(wildcard *.svg)
PNG=$(SVG:.svg=.png)

main: $(PNG)

%.png : %.svg
	inkscape -D -o $@ $^

clean:
	@rm -f $(PNG)
