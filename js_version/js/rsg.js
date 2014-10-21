/*
Chrisdona Joan C. Paraiso, Stanley Dotloe
11/23/2012
CSCE 343
Assignment 6  - Random Sentence Generator: Generator Core
*/


$(document).ready(function() {
	//authors is an array so I can change out authors for demonstration program easier
	authors = ["Stanley Dotloe","Chrisdona Joan C. Paraiso"];
	//set up a blank grammar object
	g = new Grammar();
	
	function about()
	{
		//popup box to show authors and misc. stuff
		alert("CSCE 343\n" 
			+$('h1').text()+"\n"
			+"Authors: " + authors + "\n"
			+"Date: 11/13/2012\n");
	}
	
	function madLib()
	{
		//if the <start> nonterminal isn't defined, what's the point?
		if (!g.containsNonTerminal("<start>")){
			alert("You don't have a grammar loaded, silly.");
			return;
		}
		sentence = "<start>";

		function updateDialog()
		{
			//tidy up the sentence we have so far
			sentence=sentence.replace(/ +([!?.,])/gm,"$1");
			sentence=sentence.replace(/([^!?.,])([!?.,])([^!?., ])/gm,"$1$2 $3");
			sentence=sentence.replace(/  /gm,' ').replace(/\t/gm,'');
			//make it safe for placing in the dialog box
			s = htmlspecialchars(sentence);

			//locate what we need to highlight
			lhs_start = s.indexOf('&lt;');
			lhs_end = s.indexOf('&gt;')+1+4;
			lhs = s.substring(lhs_start,lhs_end);
			if (lhs_start === -1)
			{
				//if we're done filling out the sentence, hide the expansion controls
				$('#cursen').html(s);
				$('#expansions').hide();

			}
			else
			{
				//if we're not done filling out the sentence, show the expansion controls
				//and highlight the current nonterminal we're working on
				$('#cursen').html(s.replace(lhs,'<span class="highlight">'+lhs+'</span>'));
				$('#expansions').show();
			}

			//locate what we need to replace
			lhs_start = sentence.indexOf('<');
			lhs_end = sentence.indexOf('>')+1;
			lhs = sentence.substring(lhs_start,lhs_end);
			//rebuild the select widget with all possible existing expansions
			//for the current item we're working with
			if($('select').hasClass('ui-multiselect'))
				$('select').multiselect('destroy');
			$('select#possible-expansions')[0].options.length = 0;
			expansions = g.getAllRHS(lhs);
			for (var i = 0;i<expansions.length;i++)
			{
				o = new Option(expansions[i], i);
				$('select#possible-expansions')[0].options[$('select#possible-expansions')[0].options.length]=o;
			}
			$('select').multiselect({multiple:false,selectedList:4,nonSelectedText:'Choose an expansion'});
		}

		function advance(mode)
		{
			//locate what we need to replace
			lhs_start = sentence.indexOf('<');
			lhs_end = sentence.indexOf('>')+1;
			lhs = sentence.substring(lhs_start,lhs_end);
			//get our source for the replacement
			if (mode === 0)
			{
				//if we clicked the "Fill for me", get a random expansion
				replacement=g.getRandomRHS(lhs);
			}
			else if (mode === 1)
			{
				//if we clicked the "Use selected expansion"
				//get the currently selected expansion
				idx=$('select#possible-expansions')[0].options.selectedIndex;
				replacement=g.getAllRHS(lhs)[idx];
			}
			else if (mode === 2)
			{
				//if we clicked "Use custom expansion"
				//grab the current data we have in the textbox
				replacement=$('#custom-expansion')[0].value;
			}
			//we update the sentence we're working with
			sentence=sentence.replace(lhs,replacement);
			//and go back to move to the next nonterminal we need to parse
			updateDialog();
		}
		//set up the dialog
		updateDialog();
		//present it
		$('#madlib').dialog("open");
		//add some buttons to it
		$('#madlib').dialog({ buttons: {
						Restart: function() {sentence="<start>";updateDialog();},
						'Use custom expansion': function() { advance(2); },
						'Use selected expansion': function() { advance(1); },
						'Fill for me': function() { advance(0); },
						Cancel: function() { $( this ).dialog( "close" ); } 
						}});
	}

	function randomSentence()
	{
		//if the <start> nonterminal isn't defined, what's the point?
		if (!g.containsNonTerminal("<start>")){
			$('#random-sentence').text("You don't have a grammar loaded, silly.");
			return;
		}
		sentence = "<start>";
		while (sentence.indexOf('<') >= 0)
		{
			//locate what we need to replace
			lhs_start = sentence.indexOf('<');
			lhs_end = sentence.indexOf('>')+1;
			lhs = sentence.substring(lhs_start,lhs_end);
			//now replace it with some random expansion
			sentence = sentence.replace(lhs,g.getRandomRHS(lhs));
		}
		//tidy up the sentence we have so far
		sentence=sentence.replace(/ +([!?.,])/gm,"$1");
		sentence=sentence.replace(/([^!?.,])([!?.,])([^!?., ])/gm,"$1$2 $3");
		sentence=sentence.replace(/  /gm,' ').replace(/\t/gm,'');
		//place it in the "Sentence Generated" tab and focus on that
		$('#random-sentence').html(htmlspecialchars(sentence));
		$( "#panels" ).tabs({selected:1});
	}
	
	function updateAccordion()
	{
		//autofill the collapsible accordion from
		//the grammar's contents, so that way
		//we can browse the grammar more easily
		$('#grammar-content').accordion('destroy');
		$('#grammar-content').html(g.toString());
		$('#grammar-content').accordion({heightStyle:'content'});
	}

	function loadGrammarFromText(rewrite)
	{
		//test if we're making a new grammar
		if(rewrite)
			g = new Grammar();
		//get the grammar we're parsing
		gtext=document.getElementById('grammar-input').value;
		if (gtext !== "")
		{
			//load the grammar from the form
			//and refresh it
			g.prepareFromString(gtext);
			updateAccordion();
			$( "#panels" ).tabs({selected:2});
		}
		else if (rewrite)
		{
			//otherwise, just put filler text
			$('#grammar-content').text('There is no grammar loaded.');
		}
	}
	
	function readSingleFile(evt) {
		//Retrieve the first (and only!) File from the FileList object
		var f = evt.target.files[0]; 
		
		if (f) {
			//let's try reading the file
			var r = new FileReader();
			//let the user know we loaded the file right
			r.onload = function(e) { 
				var contents = e.target.result;
				alert( "Got the file.\n" 
							+"name: " + f.name + "\n"
							+"type: " + f.type + "\n"
							+"size: " + f.size + " bytes\n"
							+ "starts with: " + contents.substr(0, 10)
				);
				//completely rewrite the grammar using file contents
				//and update accordion accordingly
				g = new Grammar();
				g.prepareFromString(e.target.result);
				updateAccordion();
				$( "#panels" ).tabs({selected:2});
			}
			r.readAsText(f);
		} else { 
			//otherwise notify failure
			//and place filler text
			alert("Failed to load file");
			$('#grammar-content').text("There is no grammar loaded.");
		}
	}
	//add function to listen to uploading file
	document.getElementById('fileinput').addEventListener('change', readSingleFile, false);
	//make the "Random sentence now!" button generate random sentence on a click
	document.getElementById('make-sentence').onclick=randomSentence;
	//make the "Madlibify" button raise the madlib dialog
	document.getElementById('make-sentencei').onclick=madLib;
	//make the about button raise the dialog
	document.getElementById('about').onclick=about;
	//make the "Add to existing grammar" button call loadGrammarFromText(false)
	document.getElementById("grammar-input-append").onclick=function(){loadGrammarFromText(false)};
	//make the "Load grammar from text" button call loadGrammarFromText(true)
	document.getElementById("grammar-input-rewrite").onclick=function(){loadGrammarFromText(true)};
	//set up the rest of the prettifying things
	$('#madlib').dialog({ autoOpen: false, minHeight: 400, minWidth: 870 });
	$( "button" ).button();
	$("#grammar-content").accordion();
	$( "#panels" ).tabs();
	$('select').multiselect({multiple:false,selectedList:4,nonSelectedText:'Choose an expansion'});
});
