# Building the C++ version
Run the following command in the shell
```
make
```

# Running
## Javascript version
* Extract all the files in the js_version folder and click on the randomSentenceGenerator.html file
* Click either "Load grammar file..." or paste the contents of your grammar file into the text box.
	* If you entered it into the textbox, either press "Add to existing grammar" or "Load grammar from text". The second one will completely rewrite the grammar based on what you typed in, the other will just add what's in the textbox to what grammar you already loaded.
* Click either "Random sentence now!" or "Madlibify" to start generating sentences.
* If you clicked the "Madlibify" button, here are some additional directions:
	* If you want the currently highlighted nonterminal to be expanded by the program, click "Fill for me"
	* If you want to use your own expansion, type in the textbox provided and click "Use custom expansion"
	* If you want to use an already existing expansion and selected it from the dropdown, click "Use selected expansion"
	* Repeat one of the previous three until there are no more nonterminals that need to be replaced.
	* Pressing "Restart" brings you back to expanding the <start> nonterminal
	* Pressing "Cancel" closes the madlib dialog
	* Also, you can drag around the dialog to take a look at the existing expansions while you fill out the madlib. You 
can use existing nonterminals even in your custom expansion.

## C++ version
```
./random_sentence <path-to-grammar-file> [{ <path-to-grammar-file>}]
```
