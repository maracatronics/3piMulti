var previouschar = 0;
var temp = "";
var sufix = "VRAU";
var linecomment = false;
var blockcomment = false;
var includeline = false;
var isString = false;

var result;

function isName(c) {
	if (c >= '0' && c <= '9') return true;
	if (c >= 'a' && c <= 'z') return true;
	if (c >= 'A' && c <= 'Z') return true;
	if (c == '_') return true;
	return false;
}

function getReservedWords(file, callback) {

    var xobj = new XMLHttpRequest();
    xobj.open('GET', file , true); // Replace 'my_data' with the path to your file
    xobj.onreadystatechange = function () {
        if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            var lines = xobj.responseText.split('\n');
            var reservedWords=[]
            for(var i=0; i<lines.length;i++){
                if(isName(lines[i].trim()[0])){
                    // console.log(lines[i].split('\t')[0]);
                    reservedWords.push(lines[i].split('\t')[0])
                }
            }
            callback(reservedWords);
        }
    }
    xobj.send(null);  
}


// usar uma hash para melhorar o desempenho
//vector<string> reservedwords;
var reservedwords = [];

function loadReservedWords() {
	var keywordsFiles = ['keywordsArd.txt', 'keywordsOrangutanAnalog.txt', 'keywordsOrangutanBuzzer.txt', 'keywordsOrangutanLCD.txt', 'keywordsOrangutanLEDs.txt', 'keywordsOrangutanMotors.txt', 'keywordsOrangutanPushbuttons.txt', 'keywordsPololu30i.txt', 'keywordsPololuWheelEncoders.txt', 'keywordsQTRSensors.txt']
	for(var i=0; i<keywordsFiles.length; i++){
		getReservedWords('assets/keywords/'+keywordsFiles[i], function(response) {
			reservedwords = reservedwords.concat(response);
		});	
	}
}

// function loadReservedWords() {
// 	reservedwords.push("main");
// 	reservedwords.push("if");
// 	reservedwords.push("else");
// 	reservedwords.push("switch");
// 	reservedwords.push("case");
// 	reservedwords.push("break");
// 	reservedwords.push("while");
// 	reservedwords.push("for");
// 	reservedwords.push("define");
// 	reservedwords.push("include");
// 	reservedwords.push("string");
// 	reservedwords.push("int");
// 	reservedwords.push("unsigned");
// 	reservedwords.push("char");
// 	reservedwords.push("float");
// 	reservedwords.push("double");
// 	reservedwords.push("const");
// 	reservedwords.push("void");
// 	reservedwords.push("Pololu3pi");
// 	reservedwords.push("PololuQTRSensors");
// 	reservedwords.push("OrangutanMotors");
// 	reservedwords.push("OrangutanAnalog");
// 	reservedwords.push("OrangutanLEDs");
// 	reservedwords.push("OrangutanLCD");
// 	reservedwords.push("OrangutanPushbuttons");
// 	reservedwords.push("OrangutanBuzzer");
// 	reservedwords.push("PROGMEM");
// 	reservedwords.push("init");
// 	reservedwords.push("loadCustomCharacter");
// 	reservedwords.push("clear");
// 	reservedwords.push("print");
// 	reservedwords.push("printFromProgramSpace");
// 	reservedwords.push("gotoXY");
// 	reservedwords.push("delay");
// 	reservedwords.push("isPressed");
// 	reservedwords.push("readBatteryMillivolts");
// 	reservedwords.push("waitForRelease");
// 	reservedwords.push("BUTTON_A");
// 	reservedwords.push("BUTTON_B");
// 	reservedwords.push("setSpeeds");
// 	reservedwords.push("calibrateLineSensors");
// 	reservedwords.push("readLine");
// 	reservedwords.push("IR_EMITTERS_ON");
// 	reservedwords.push("isPlaying");
// 	reservedwords.push("left");
// 	reservedwords.push("right");
// 	reservedwords.push("LOW");
// 	reservedwords.push("HIGH");
// 	reservedwords.push("playFromProgramSpace");
// }

function isReserved(s) {
	for (var i = 0; i < reservedwords.length; i++) {
		if (s == reservedwords[i]) return true;
	}
	return false;
}

function processName() {
	var c = temp.charAt(0);
	if (!(c >= '0' && c <= '9')) {
		if (!linecomment && !blockcomment && !includeline && !isString) {
			if (!isReserved(temp)) {
				temp += sufix;
			}
		}
	}
	
	//printf("%s", temp.c_str());
	result += temp;
	temp = "";
}

function process(c) {
	if (isName(c)) {
		temp += c;
	} else {
		if (c == '*' && previouschar == '/') {
			blockcomment = true;
		} else if (c == '/' && previouschar == '*') {
			blockcomment = false;
		} else if (c == '/' && previouschar == '/') {
			linecomment = true;
		} else if (c == '\n') {
			linecomment = false;
			includeline = false;
			isString = false;
		} else if (c == 'i' && previouschar == '#') {
			includeline = true;
		}

		if (temp.length != 0) {
			processName();
		}

		if (c == '\"') { // '"'
			isString = !isString;
		}
		
		//printf("%c", c);
		result += c;
	}
	previouschar = c;
}

function processFile(filename, filecontent, index) {
	
	//printf("\n\n/*----- %s -----*/\n\n", filename.c_str());
	result += "\n\n/*----- " + filename + " -----*/\n\n";

	//var tempsufix[100];
	//sprintf(tempsufix, "%03d", index);
	sufix = "" + index;

	//FILE *f = fopen(filename.c_str(), "rb");

	var c;

	for(var i = 0; i < filecontent.length; i++) {
		process(filecontent.charAt(i));	
	}
	process('\n');
}

function createMenu(numPrograms) {

	result += "\n\n/*----- MENU -----*/\n\n";

	result += "void(*mainsetup)();\n";
	result += "void(*mainloop)();\n";
	result += "\n";
	
	result += "void setup() {\n";
	result += "\tint option = 0;\n";
	result += "\tbool selected = false;\n";
	result += "\twhile (!selected) {\n";

	result += "\t\tOrangutanLCD::clear();\n";
    result += "\t\tOrangutanLCD::gotoXY(0, 0);\n";
    result += "\t\tOrangutanLCD::print(\"Prog. \");\n";
    result += "\t\tOrangutanLCD::print(option + 1);\n";
    result += "\t\tOrangutanLCD::gotoXY(0, 1);\n";

	result += "\t\tswitch (option) {\n";
	
	var keys = Object.keys(codes);
	for(var i = 0; i < numPrograms; i++) {
		result += "\t\t\tcase " + i + ": OrangutanLCD::print(\""  + keys[i] + "\"); break;\n";
	}
	
	result += "\t\t\tdefault: break;\n";
	result += "\t\t}\n";


	result += "\t\tif(OrangutanPushbuttons::isPressed(BUTTON_A)) {\n";
	result += "\t\t\tOrangutanPushbuttons::waitForRelease(BUTTON_A);\n";
	result += "\t\t\toption = (option - 1) % " + numPrograms + ";\n";
	result += "\t\t}\n";
	result += "\t\telse if (OrangutanPushbuttons::isPressed(BUTTON_B)) {\n";
	result += "\t\t\tOrangutanPushbuttons::waitForRelease(BUTTON_B);\n";
	result += "\t\t\toption = (option + 1) % " + numPrograms + ";\n";
	result += "\t\t}\n";
	result += "\t\telse if (OrangutanPushbuttons::isPressed(BUTTON_C)) {\n";
	result += "\t\t\tOrangutanPushbuttons::waitForRelease(BUTTON_C);\n";
	result += "\t\t\tswitch (option) {\n";
	
	for(var i = 0; i < numPrograms; i++) {
		result += "\t\t\t\tcase " + i + ": mainsetup = &setup" + i + "; mainloop = &loop" + i + "; break;\n";
	}
	
	result += "\t\t\t\tdefault: break;\n";
	result += "\t\t\t}\n";
	result += "\t\t\tselected = true;\n";
	result += "\t\t}\n";
	result += "\t\tdelay(100);\n";
	result += "\t}\n";
	result += "\tmainsetup();\n";
	result += "}\n";
	
	result += "\n";
	result += "void loop() {\n";
	result += "\tmainloop();\n";
	result += "}\n";

}

// function main() {

// 	loadReservedWords();

// 	//processFile("Simple1.ino", 0);
// 	//processFile("Battery.ino", 1);
// 	//processFile("Simple3piLineFollower.ino", 2);

// 	createMenu(3);
// }