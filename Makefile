all: clean
	@g++ -o salida_de_plot tarea1_sisEvol.c -lglut -lGLU -lGL -lm
run: all 
	@./salida_de_plot
clean:
	@rm salida_de_plot || true
