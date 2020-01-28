# GameOfLife

Implementation of the famous Game of Life in C language. You can read more information about the game [here](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).


This project was created during the first semester of computer science studies for the Basics of Programming course. 
That required it to be written in C language. Moreover, both the comments to the code and [documentation](https://github.com/psatala/GameOfLife/blob/master/Documentation/Dokumentacja%20Projekt%203%20PRI.pdf) are written partially in polish.

## Compilation ##

To compile the program on your computer, first ensure that you have installed the <i>ncurses</i> library. 
It provides application programming interface that allows to create text-based user interface.
To install it on Ubuntu, type:\
<code>sudo apt-get install libncurses-dev</code>

After the installation, type
<code>make</code> to compile the program.

## Execution ##

Once the program has been compiled, type <code>./GameOfLife</code> to run it. To navigate the menu use arrow keys. Further instructions will be provided by the program. 

You may however start the application with additional arguments:
* name of an input file. It will be used to create the board for the game, such as [those files](https://github.com/psatala/GameOfLife/tree/master/Examples).
* name of an output file. You can store the output in the form of <i>txt</i> or <i>html</i> file.

e.g. <code>./GameOfLife Examples/glider_gun Outputs/glider_gun.html</code>
