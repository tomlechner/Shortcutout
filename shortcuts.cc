//
// Shortcut window test
//

#include <lax/anxapp.h>
#include <lax/language.h>
#include <lax/filedialog.h>
#include <lax/fileutils.h>
#include <lax/utf8string.h>

#include <lax/shortcutwindow2.h>

#include <iostream>

//TODO
//
// save/load window layout with proper stack frame metrics
// inkscape:
//   better key capitalization
//   DONE deal with the "display" attribute
// krita:
//   in should parse dups "Action=k1; k2"
//   out should output dups (only 2)
//   needs proper key to string
// html version:
//   key input updates search to have modded keys only
//   progressive search
//   click key searches all for that key, any mods
//   sort by key, action, or description
// keypad shortcuts not really doing correct thing
// optionally have ability to add action command based on arbtrary string data
// export svg: with action tags in tiny writing
// export html, use existing shortcut manager svg + svg keyboard? maybe leverage shortcutmapper?
// detect current physical layout
// keyboard loading
// dedicated search box
// action list needs right click clear?
// needs column access for sorting action list
// define new action
//
// standalone to change keys for:
//   inkscape
//   gimp
//   scribus
//   darktable
//   krita
//   blender
//   godot
//
// DONE load in from ShortcutMapper
// DONE clicking on action with key should set mods on keyboard and highlight the key
// DONE drag actions up from action list
// DONE action window needs better input key now indicator
// DONE laidout integration
// DONE use current locale
// DONE area window needs cursor select
// DONE action tooltip
// DONE load and save key sets
// DONE Clear key from menu
// DONE select action from rb menu
// DONE area list needs to trigger action list update and update of tags
// 



using namespace Laxkit;
using namespace LaxFiles;
using namespace std;


#define DBG


//------------------------- ShortcutMapper import -----------------------------

void LaxToSMKey(char *keyname, unsigned int k)
{
	if (k == 0) return;
	keyname[0] = '\0';
	
	//'<,'>s/\("[^"]*"\)) return \([^;]*\);/k == \2) return \1;

	if (k == '`') strcpy(keyname, "ACCENT_GRAVE");
	if (k == LAX_Alt) strcpy(keyname, "ALT");
	if (k == '*') strcpy(keyname, "ASTERISK");
	if (k == '\\') strcpy(keyname, "BACKSLASH");
	if (k == LAX_Bksp) strcpy(keyname, "BACKSPACE");
	if (k == LAX_Capslock) strcpy(keyname, "CAPSLOCK");
	if (k == ':') strcpy(keyname, "COLON");
	if (k == ',') strcpy(keyname, "COMMA");
	if (k == LAX_Meta) strcpy(keyname, "COMMAND");
	if (k == LAX_Control) strcpy(keyname, "CONTROL");
	if (k == LAX_Del) strcpy(keyname, "DELETE");
	if (k == LAX_Down) strcpy(keyname, "DOWN_ARROW");
	if (k == '8') strcpy(keyname, "EIGHT");
	if (k == LAX_End) strcpy(keyname, "END");
	if (k == LAX_Enter) strcpy(keyname, "ENTER");
	if (k == '=') strcpy(keyname, "EQUAL");
	if (k == LAX_Esc) strcpy(keyname, "ESCAPE");
	if (k == '!') strcpy(keyname, "EXCLAMATION");
	if (k == '5') strcpy(keyname, "FIVE");
	if (k == '4') strcpy(keyname, "FOUR");
	if (k == '#') strcpy(keyname, "HASH");
	if (k == LAX_Home) strcpy(keyname, "HOME");
	if (k == LAX_Ins) strcpy(keyname, "INSERT");
	if (k == LAX_Left) strcpy(keyname, "LEFT_ARROW");
	if (k == '{') strcpy(keyname, "LEFT_BRACE");
	if (k == '[') strcpy(keyname, "LEFT_BRACKET");
	if (k == '<') strcpy(keyname, "LESSTHAN");
	if (k == '-') strcpy(keyname, "MINUS");
	if (k == '>') strcpy(keyname, "MORETHAN");
	if (k == '9') strcpy(keyname, "NINE");
	if (k == '*') strcpy(keyname, "NUMPAD_ASTERISK");
	if (k == '8') strcpy(keyname, "NUMPAD_EIGHT");
	if (k == LAX_Enter) strcpy(keyname, "NUMPAD_ENTER");
	if (k == '=') strcpy(keyname, "NUMPAD_EQUAL");
	if (k == '5') strcpy(keyname, "NUMPAD_FIVE");
	if (k == '4') strcpy(keyname, "NUMPAD_FOUR");
	if (k == '-') strcpy(keyname, "NUMPAD_MINUS");
	if (k == '9') strcpy(keyname, "NUMPAD_NINE");
	if (k == '1') strcpy(keyname, "NUMPAD_ONE");
	if (k == '.') strcpy(keyname, "NUMPAD_PERIOD");
	if (k == '+') strcpy(keyname, "NUMPAD_PLUS");
	if (k == '7') strcpy(keyname, "NUMPAD_SEVEN");
	if (k == '6') strcpy(keyname, "NUMPAD_SIX");
	if (k == '/') strcpy(keyname, "NUMPAD_SLASH");
	if (k == '3') strcpy(keyname, "NUMPAD_THREE");
	if (k == '2') strcpy(keyname, "NUMPAD_TWO");
	if (k == '0') strcpy(keyname, "NUMPAD_ZERO");
	if (k == '1') strcpy(keyname, "ONE");
	if (k == LAX_Pgdown) strcpy(keyname, "PAGE_DOWN");
	if (k == LAX_Pgup) strcpy(keyname, "PAGE_UP");
	if (k == LAX_Pause) strcpy(keyname, "PAUSE_BREAK");
	if (k == '.') strcpy(keyname, "PERIOD");
	if (k == '+') strcpy(keyname, "PLUS");
	if (k == '?') strcpy(keyname, "QUESTION_MARK");
	if (k == LAX_Right) strcpy(keyname, "RIGHT_ARROW");
	if (k == '}') strcpy(keyname, "RIGHT_BRACE");
	if (k == ']') strcpy(keyname, "RIGHT_BRACKET");
	if (k == LAX_ScrollLock) strcpy(keyname, "SCROLL_LOCK");
	if (k == ';') strcpy(keyname, "SEMICOLON");
	if (k == '7') strcpy(keyname, "SEVEN");
	if (k == LAX_Shift) strcpy(keyname, "SHIFT");
	if (k == '\'') strcpy(keyname, "SINGLE_QUOTE");
	if (k == '6') strcpy(keyname, "SIX");
	if (k == '/') strcpy(keyname, "SLASH");
	if (k == ' ') strcpy(keyname, "SPACE");
	if (k == '\t') strcpy(keyname, "TAB");
	if (k == '3') strcpy(keyname, "THREE");
	if (k == '~') strcpy(keyname, "TILDE");
	if (k == '2') strcpy(keyname, "TWO");
	if (k == LAX_Up) strcpy(keyname, "UP_ARROW");
	if (k == '0') strcpy(keyname, "ZERO");
	//if (k == 0) strcpy(keyname, "MEDIA_NEXT");
	//if (k == 0) strcpy(keyname, "MEDIA_PLAY");
	//if (k == 0) strcpy(keyname, "MEDIA_PREVIOUS");
	//if (k == 0) strcpy(keyname, "MEDIA_STOP");
	if (k == LAX_F1 ) strcpy(keyname, "F1");
	if (k == LAX_F2 ) strcpy(keyname, "F2");
	if (k == LAX_F3 ) strcpy(keyname, "F3");
	if (k == LAX_F4 ) strcpy(keyname, "F4");
	if (k == LAX_F5 ) strcpy(keyname, "F5");
	if (k == LAX_F6 ) strcpy(keyname, "F6");
	if (k == LAX_F7 ) strcpy(keyname, "F7");
	if (k == LAX_F8 ) strcpy(keyname, "F8");
	if (k == LAX_F9 ) strcpy(keyname, "F9");
	if (k == LAX_F10) strcpy(keyname, "F10");
	if (k == LAX_F11) strcpy(keyname, "F11");
	if (k == LAX_F12) strcpy(keyname, "F12");

	if (keyname[0] == '\0' && k >= 32 && k < 128) sprintf(keyname, "%c", toupper(k));
}

void LaxToXlibKey(char *keyname, unsigned int key)
{
	LaxToSMKey(keyname, key);
	
	char *p = keyname;
	while (*p) {
		*p = tolower(*p);
		p++;
	}
}

unsigned int SMKeyToLax(const char *k)
{
	if (!k || *k==0) return 0;
	
	if (!strcasecmp(k, "ACCENT_GRAVE")) return '`';
	if (!strcasecmp(k, "ALT")) return LAX_Alt;
	if (!strcasecmp(k, "ASTERISK")) return '*';
	if (!strcasecmp(k, "BACKSLASH")) return '\\';
	if (!strcasecmp(k, "BACKSPACE")) return LAX_Bksp;
	if (!strcasecmp(k, "CAPSLOCK")) return LAX_Capslock;
	if (!strcasecmp(k, "COLON")) return ':';
	if (!strcasecmp(k, "COMMA")) return ',';
	if (!strcasecmp(k, "COMMAND")) return LAX_Meta;
	if (!strcasecmp(k, "CONTROL")) return LAX_Control;
	if (!strcasecmp(k, "DELETE")) return LAX_Del;
	if (!strcasecmp(k, "DOWN_ARROW")) return LAX_Down;
	if (!strcasecmp(k, "EIGHT")) return '8';
	if (!strcasecmp(k, "END")) return LAX_End;
	if (!strcasecmp(k, "ENTER")) return LAX_Enter;
	if (!strcasecmp(k, "EQUAL")) return '=';
	if (!strcasecmp(k, "ESCAPE")) return LAX_Esc;
	if (!strcasecmp(k, "EXCLAMATION")) return '!';
	if (!strcasecmp(k, "FIVE")) return '5';
	if (!strcasecmp(k, "FOUR")) return '4';
	if (!strcasecmp(k, "HASH")) return '#';
	if (!strcasecmp(k, "HOME")) return LAX_Home;
	if (!strcasecmp(k, "INSERT")) return LAX_Ins;
	if (!strcasecmp(k, "LEFT_ARROW")) return LAX_Left;
	if (!strcasecmp(k, "LEFT_BRACE")) return '{';
	if (!strcasecmp(k, "LEFT_BRACKET")) return '[';
	if (!strcasecmp(k, "LESSTHAN")) return '<';
	if (!strcasecmp(k, "MINUS")) return '-';
	if (!strcasecmp(k, "MORETHAN")) return '>';
	if (!strcasecmp(k, "NINE")) return '9';
	if (!strcasecmp(k, "NUMPAD_ASTERISK")) return '*';
	if (!strcasecmp(k, "NUMPAD_EIGHT")) return '8';
	if (!strcasecmp(k, "NUMPAD_ENTER")) return LAX_Enter;
	if (!strcasecmp(k, "NUMPAD_EQUAL")) return '=';
	if (!strcasecmp(k, "NUMPAD_FIVE")) return '5';
	if (!strcasecmp(k, "NUMPAD_FOUR")) return '4';
	if (!strcasecmp(k, "NUMPAD_MINUS")) return '-';
	if (!strcasecmp(k, "NUMPAD_NINE")) return '9';
	if (!strcasecmp(k, "NUMPAD_ONE")) return '1';
	if (!strcasecmp(k, "NUMPAD_PERIOD")) return '.';
	if (!strcasecmp(k, "NUMPAD_PLUS")) return '+';
	if (!strcasecmp(k, "NUMPAD_SEVEN")) return '7';
	if (!strcasecmp(k, "NUMPAD_SIX")) return '6';
	if (!strcasecmp(k, "NUMPAD_SLASH")) return '/';
	if (!strcasecmp(k, "NUMPAD_THREE")) return '3';
	if (!strcasecmp(k, "NUMPAD_TWO")) return '2';
	if (!strcasecmp(k, "NUMPAD_ZERO")) return '0';
	if (!strcasecmp(k, "ONE")) return '1';
	if (!strcasecmp(k, "PAGE_DOWN")) return LAX_Pgdown;
	if (!strcasecmp(k, "PAGE_UP")) return LAX_Pgup;
	if (!strcasecmp(k, "PAUSE_BREAK")) return LAX_Pause;
	if (!strcasecmp(k, "PERIOD")) return '.';
	if (!strcasecmp(k, "PLUS")) return '+';
	if (!strcasecmp(k, "QUESTION_MARK")) return '?';
	if (!strcasecmp(k, "RIGHT_ARROW")) return LAX_Right;
	if (!strcasecmp(k, "RIGHT_BRACE")) return '}';
	if (!strcasecmp(k, "RIGHT_BRACKET")) return ']';
	if (!strcasecmp(k, "SCROLL_LOCK")) return LAX_ScrollLock;
	if (!strcasecmp(k, "SEMICOLON")) return ';';
	if (!strcasecmp(k, "SEVEN")) return '7';
	if (!strcasecmp(k, "SHIFT")) return LAX_Shift;
	if (!strcasecmp(k, "SINGLE_QUOTE")) return '\'';
	if (!strcasecmp(k, "SIX")) return '6';
	if (!strcasecmp(k, "SLASH")) return '/';
	if (!strcasecmp(k, "SPACE")) return ' ';
	if (!strcasecmp(k, "TAB")) return '\t';
	if (!strcasecmp(k, "THREE")) return '3';
	if (!strcasecmp(k, "TILDE")) return '~';
	if (!strcasecmp(k, "TWO")) return '2';
	if (!strcasecmp(k, "UP_ARROW")) return LAX_Up;
	if (!strcasecmp(k, "ZERO")) return '0';
	if (!strcasecmp(k, "F1"))  return LAX_F1;
	if (!strcasecmp(k, "F2"))  return LAX_F2;
	if (!strcasecmp(k, "F3"))  return LAX_F3;
	if (!strcasecmp(k, "F4"))  return LAX_F4;
	if (!strcasecmp(k, "F5"))  return LAX_F5;
	if (!strcasecmp(k, "F6"))  return LAX_F6;
	if (!strcasecmp(k, "F7"))  return LAX_F7;
	if (!strcasecmp(k, "F8"))  return LAX_F8;
	if (!strcasecmp(k, "F9"))  return LAX_F9;
	if (!strcasecmp(k, "F10")) return LAX_F10;
	if (!strcasecmp(k, "F11")) return LAX_F11;
	if (!strcasecmp(k, "F12")) return LAX_F12;

	// hmm..
	if (!strcasecmp(k, "MEDIA_NEXT")) return 0;
	if (!strcasecmp(k, "MEDIA_PLAY")) return 0;
	if (!strcasecmp(k, "MEDIA_PREVIOUS")) return 0;
	if (!strcasecmp(k, "MEDIA_STOP")) return 0;
	
	if (k[1]=='\0' && k[0]>='A' && k[0]<='Z') return 'a' + (int)k[0] - 'A'; //ascii printable

	if (k[1]=='\0' && k[0]>=32 && k[0]<128) return (int)k[0]; //ascii printable
	if (k[0]!='u' && k[0]!='U') return 0;
	return strtol(k+1,NULL,16);
}

/*! See http://waldobronchart.github.io/ShortcutMapper
 */
int ImportShortcutMapper(const char *file, string &default_ctx)
{
	Attribute jsonatt;
	Attribute *att, *att2;
	char *json = read_in_whole_file(file, nullptr);
	int status = jsonatt.dump_in_json(json);
	delete[] json;
	if (status) return status;
	att = &jsonatt;

	ShortcutManager *manager = GetDefaultShortcutManager();
	makestr(manager->setfile, file);
	const char *default_context;

	att = &jsonatt;
	if (strcmp(att->name, "object")) return 1;

	int actionnumber = 0;
	const char *name, *value;
	for (int c=0; c<att->attributes.n; c++) {
		name  = att->attributes.e[c]->name;
		value = att->attributes.e[c]->value;

		
//    "name" : "Blender",
//    "version" : "v2.78",
//    "os" : "windows",
//    "mods_used" : ["ALT", "COMMAND", "CONTROL", "SHIFT"],
//    "default_context" : "3D View",
//    "contexts" : {
//        "3D View" : {
//            "A" : [

		if (!strcmp(value, "name")) { //name was "key"
			makestr(manager->setname, att->attributes.e[c]->attributes.e[0]->value);

		} else if (!strcmp(value, "version")) {
			appendstr(manager->setname, " ");
			appendstr(manager->setname, att->attributes.e[c]->attributes.e[0]->value);

		} else if (!strcmp(value, "os")) {
			appendstr(manager->setname, ", ");
			appendstr(manager->setname, att->attributes.e[c]->attributes.e[0]->value);

		} else if (!strcmp(value, "mods_used")) {

		} else if (!strcmp(value, "default_context")) {
			default_context = att->attributes.e[c]->attributes.e[0]->value;
			default_ctx = default_context;

		} else if (!strcmp(value, "contexts")) {
			att2 = att->attributes.e[c]->find("object");

			for (int c2=0; c2<att2->attributes.n; c2++) {
				name  = att2->attributes.e[c2]->name; //should be "key"
				value = att2->attributes.e[c2]->value; //should be a context name
				
				const char *contextname = value;
				Laxkit::ShortcutHandler *handler = new Laxkit::ShortcutHandler(value); 

				Attribute *att3 = att2->attributes.e[c2]->find("object"); //should be hash of keys

				for (int c3=0; c3<att3->attributes.n; c3++) {
					name  = att3->attributes.e[c3]->name;  //should be "key"
					value = att3->attributes.e[c3]->value; //should be the key name

					const char *keyname = value;
					unsigned int ch = SMKeyToLax(keyname);

					Attribute *att4 = att3->attributes.e[c3]->find("array");

					// new key name, children should be "object" like: {"name":"Set Active Keying Set", "mods":["ALT", "CONTROL", "SHIFT"]}
					for (int c4=0; c4<att4->attributes.n; c4++) {
						//name  = att4->attributes.e[c4]->name;
						//value = att4->attributes.e[c4]->value;

						Attribute *att5 = att4->attributes.e[c4];
						if (strcmp(att5->name, "object")) continue;

						const char *description = nullptr;
						unsigned int mods = 0;

						for (int c5=0; c5<att5->attributes.n; c5++) {
							name  = att5->attributes.e[c5]->name; //should be "key"
							value = att5->attributes.e[c5]->value;

							if (!strcmp(value, "name")) {
								description = att5->attributes.e[c5]->attributes.e[0]->value;

							} else if (!strcmp(value, "mods")) {
								Attribute *att6 = att5->attributes.e[c5]->attributes.e[0];

								for (int c6=0; c6<att6->attributes.n; c6++) {
									name  = att6->attributes.e[c6]->name;
									value = att6->attributes.e[c6]->value;

									if (!strcmp(value, "SHIFT")) mods |= ShiftMask;
									else if (!strcmp(value, "ALT")) mods |= AltMask;
									else if (!strcmp(value, "COMMAND")) mods |= MetaMask;
									else if (!strcmp(value, "CONTROL")) mods |= ControlMask;
									//else if (!strcmp(value, "SPACE")) mods |= ***Mask; wtf photoshop with the space as modifier!?
								}
							}
						}

						handler->Add((actionnumber++), ch,mods,0, description, description, nullptr,0);
					}
				}
				
				manager->AddArea(contextname, handler);
			}
		}
	}

	return 0;
}

int ExportShortcutMapper(Keyboard *kb, const char *file)
{
	DBG cerr << "Export shortcutmapper..."<<endl;

	ShortcutManager *manager = GetDefaultShortcutManager();
	if (manager->shortcuts.n == 0) return 1;

	FILE *f = fopen(file, "w");
	if (!f) return 1;

	fprintf(f, "{\n"
			   "  \"name\" : \"%s\",\n"
			   "  \"version\" : \"%s\",\n"
			   "  \"os\" : \"%s\",\n"
			   "  \"mods_used\" : [\"ALT\", \"CONTROL\", \"SHIFT\"],\n"
			   "  \"default_context\" : \"%s\",\n"
		       "  \"contexts\" : {\n",
			   manager->setname,
			   manager->settitle, //version
			   manager->subtitle, //os
			   manager->shortcuts.e[0]->area //default context
			   );

	char keyname[100];

	for (int c=0; c<manager->shortcuts.n; c++) {
		ShortcutDefs  *s = manager->shortcuts.e[c]->Shortcuts();
	    WindowActions *a = manager->shortcuts.e[c]->Actions();

		fprintf(f,"    \"%s\" : {\n", manager->shortcuts.e[c]->area);

		 //output all bound keys. Each physical key gets a list of action+mods
		if (s && a) {
			int nfoundkeys = 0;
			for (int kk=0; kk<kb->keys.n; kk++) {
				Key *key = kb->keys.e[kk];
				bool found = false;
				int nmaps = 0;

				for (int c2=0; c2<s->n; c2++) {
					KeyInfo * ki = s->e[c2]->keys;
					Keymap *kmap = key->HasKey(ki->key);
					if (!kmap) continue;
					
					WindowAction *aa = a->FindAction(s->e[c2]->action);

					//   ------per key------
					//"EIGHT" : [
					//  {"name":"Load Selection 8", "mods":["CONTROL", "SHIFT"]},
					//  {"name":"Save Selection 8", "mods":["ALT", "CONTROL"]}
					//],

					if (!found) {
						//key header
						found = true;
						LaxToSMKey(keyname, s->e[c2]->keys->key);
						//DBG cout << "compare key: "<<s->e[c2]->keys->key<<" against ` "<<(int)'`'<<" "<<keyname<<endl;
						if (nfoundkeys) fprintf(f, ",\n");
						fprintf(f,"      \"%s\" : [\n", keyname);
						nfoundkeys++;
					}

					//action + mods for current shortcut
					if (nmaps) fputs(",\n", f);
					fprintf(f, "        { \"name\":\"%s\", \"mods\": [",
							aa->description);
							//aa->name

					int n=0;
					if (s->e[c2]->keys->state & ShiftMask)   { n++; fputs("\"SHIFT\"", f); }
					if (s->e[c2]->keys->state & ControlMask) { n++; fprintf(f, "%s\"CONTROL\"", n>1 ? ", " : ""); }
					if (s->e[c2]->keys->state & AltMask)     { n++; fprintf(f, "%s\"ALT\"",     n>1 ? ", " : ""); }
					if (s->e[c2]->keys->state & MetaMask)    { n++; fprintf(f, "%s\"OS\"",      n>1 ? ", " : ""); }
					fputs("] }", f);

					nmaps++;
				}
				if (nmaps) fputs("\n", f);
				if (found) {
					fprintf(f, "      ]"); //end of key
				}
			}
			if (nfoundkeys) fprintf(f, "\n");
		}

		fprintf(f, "    }%s\n", c == manager->shortcuts.n-1 ? "" : ","); //end of context
        
	}

	fprintf(f, "  }\n}\n");

	fclose(f);
	return 0;
}


//----------------------------- Scribus -------------------------

int ImportScribus(const char *file)
{
	Attribute xmlatt;
	Attribute *att = XMLFileToAttribute(&xmlatt, file, nullptr);
	if (!att) return 1;

	att = att->find("shortcutset");
	if (!att) return 1;

	ShortcutManager *manager = GetDefaultShortcutManager();
	makestr(manager->setfile, file);
	makestr(manager->setname, "Scribus");
	Laxkit::ShortcutHandler *handler = new Laxkit::ShortcutHandler("Scribus"); 

	int actionnumber = 0;
	const char *name, *value;
	for (int c=0; c<att->attributes.n; c++) {
		name  = att->attributes.e[c]->name;
		value = att->attributes.e[c]->value;

		if (!strcmp(name, "name")) {
			makestr(manager->setname, value);

		} else if (!strcmp(name, "content:")) {
			for (int c2=0; c2<att->attributes.e[c]->attributes.n; c2++) {
				name  = att->attributes.e[c]->attributes.e[c2]->name; //should be "key"
				value = att->attributes.e[c]->attributes.e[c2]->value; //should be a context name

				if (strcmp(name, "function")) continue;

				unsigned int ch = 0;
				unsigned int mods = 0;
				const char *desc = nullptr;
				bool display = false;

				Attribute *att2 = att->attributes.e[c]->attributes.e[c2];
				for (int c3=0; c3<att2->attributes.n; c3++) {
					name  = att2->attributes.e[c3]->name; //should be "key"
					value = att2->attributes.e[c3]->value; //should be a context name

					if (!strcmp(name, "shortcut")) {
						while (1) {
							if (strstr(value, "Ctrl+")) { value += 5; mods |= ControlMask; }
							if (strstr(value, "Alt+"))   { value += 4; mods |= AltMask; }
							if (strstr(value, "Shift+")) { value += 6; mods |= ShiftMask; }
							break;
						}

						value = XlibToNormal(value);
						ch = key_value_from_name(value);

					} else if (!strcmp(name, "name")) {
						desc = value;
					}
				}

				if (ch != 0) {
					if (ch >= 'A' && ch <= 'Z' && (mods & ShiftMask) == 0) ch -= 'A'-'a';
					if (ch >= 'a' && ch <= 'z' && (mods & ShiftMask) != 0) ch += 'A'-'a';
					handler->Add(actionnumber++, ch,mods,0, desc, desc,NULL,0);
				} else {
					handler->AddAction(actionnumber++, desc, desc, nullptr, 0,0);
				}
			}
		}
	}

	manager->AddArea("Scribus",handler);
	return 1;
}

int ExportScribus(const char *file)
{
	cerr << " export Scribus..."<<endl;

	ShortcutManager *manager = GetDefaultShortcutManager();
	if (manager->shortcuts.n == 0) return 1;

	FILE *f = fopen(file, "w");
	if (!f) return 1;

	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			   "<shortcutset name=\"%s\">\n", manager->setname);

	char keyname[100];

	if (manager->shortcuts.n > 1) {
		cerr << "Warning!! Exporting to Scribus key format, but more than one shortcut area!" << endl;
	}

	for (int c=0; c<manager->shortcuts.n; c++) {
		ShortcutDefs  *s = manager->shortcuts.e[c]->Shortcuts();
	    WindowActions *a = manager->shortcuts.e[c]->Actions();

		 //output all bound keys. Each physical key gets a list of action+mods
		if (!s || !a) continue;


		//all bound keys:
		for (int c2=0; c2<s->n; c2++) {
			KeyInfo * ki = s->e[c2]->keys;
			WindowAction *aa = a->FindAction(s->e[c2]->action);

			fprintf(f,"  <function shortcut=\"");

			if (s->e[c2]->keys->state & ShiftMask)   { fputs("Shift+", f); }
			if (s->e[c2]->keys->state & ControlMask) { fputs("Ctrl+",  f); }
			if (s->e[c2]->keys->state & AltMask)     { fputs("Alt+" ,  f); }
			if (s->e[c2]->keys->state & MetaMask)    { fputs("Meta+",  f); }

			LaxToXlibKey(keyname, s->e[c2]->keys->key);
			fprintf(f, "%s\" name=\"%s\" />\n", keyname, aa->description);
		}
		 
//		//write out all unbound actions: ...unnecessary?
//		for (int c2=0; c2<a->n; c2++) {
//			WindowAction *aa = a->e[c2];
//			if (s->FindShortcutFromAction(aa->id,0)) continue; //already output above
//
//			fprintf(f,"; (gtk_accel_path \"%s\" \"\")\n", aa->description);
//		}
	}

	fputs("</shortcutset>\n", f);
	fclose(f);
	return 0;
}


//------------------------------- Inkscape ------------------------------------

int ImportInkscape(const char *file)
{
	Attribute xmlatt;
	Attribute *att = XMLFileToAttribute(&xmlatt, file, nullptr);
	if (!att) return 1;

	att = att->find("keys");
	if (!att) return 1;

	ShortcutManager *manager = GetDefaultShortcutManager();
	makestr(manager->setfile, file);
	makestr(manager->setname, "Inkscape");
	Laxkit::ShortcutHandler *handler = new Laxkit::ShortcutHandler("Inkscape"); 
	Utf8String scratch;

	int actionnumber = 0;
	const char *name, *value;
	for (int c=0; c<att->attributes.n; c++) {
		name  = att->attributes.e[c]->name;
		value = att->attributes.e[c]->value;

		if (!strcmp(name, "name")) {
			makestr(manager->setname, value);

		} else if (!strcmp(name, "content:")) {
			for (int c2=0; c2<att->attributes.e[c]->attributes.n; c2++) {
				name  = att->attributes.e[c]->attributes.e[c2]->name; //should be "key"
				value = att->attributes.e[c]->attributes.e[c2]->value; //should be a context name

				if (strcmp(name, "bind")) continue;

				unsigned int ch = 0;
				unsigned int mods = 0;
				const char *desc = nullptr;
				bool display = false;
				scratch.Clear();

				Attribute *att2 = att->attributes.e[c]->attributes.e[c2];
				for (int c3=0; c3<att2->attributes.n; c3++) {
					name  = att2->attributes.e[c3]->name; //should be "key"
					value = att2->attributes.e[c3]->value; //should be a context name

					if (!strcmp(name, "key")) {
						value = XlibToNormal(value);
						ch = key_value_from_name(value);

					} else if (!strcmp(name, "modifiers")) {
						if (strstr(value, "Ctrl"))  mods |= ControlMask;
						if (strstr(value, "Alt"))   mods |= AltMask;
						if (strstr(value, "Shift")) mods |= ShiftMask;

					} else if (!strcmp(name, "action")) {
						desc = value;
						scratch = desc;
						for (int c4=1; c4<scratch.Bytes(); c4++) {
							char b = scratch.byte(c4);
							char b2 = scratch.byte(c4-1);
							if (isupper(b) && islower(b2)) {
								scratch.InsertAt(" ", c4);
								c4++;
							}
						}

					} else if (!strcmp(name, "display")) {
						display = BooleanAttribute(value);
					}
				}

				if (ch != 0) {
					handler->Add(actionnumber++, ch,mods,0, scratch.c_str(), scratch.c_str(), NULL,0);
					int i = handler->FindShortcutIndex(ch,mods,0);
					handler->Shortcut(i)->info1 = display;
				} else {
					handler->AddAction(actionnumber++, scratch.c_str(), scratch.c_str(), nullptr, 0,0);
				}
			}
		}
	}

	manager->AddArea("Inkscape",handler);
	return 1;
}

int ExportInkscape(const char *file)
{
	DBG cerr << " export inkscape..."<<endl;

	ShortcutManager *manager = GetDefaultShortcutManager();
	if (manager->shortcuts.n == 0) return 1;

	FILE *f = fopen(file, "w");
	if (!f) return 1;

	fprintf(f, "<?xml version=\"1.0\"?>\n"
			   "<keys name=\"%s\">\n", manager->setname);

	char keyname[100];

	if (manager->shortcuts.n > 1) {
		cerr << "Warning!! Exporting to Inkscape key format, but more than one shortcut area!" << endl;
	}

	Utf8String scratch;

	for (int c=0; c<manager->shortcuts.n; c++) {
		ShortcutDefs  *s = manager->shortcuts.e[c]->Shortcuts();
	    WindowActions *a = manager->shortcuts.e[c]->Actions();

		 //output all bound keys. Each physical key gets a list of action+mods
		if (!s || !a) continue;


		//all bound keys:
		for (int c2=0; c2<s->n; c2++) {
			KeyInfo * ki = s->e[c2]->keys;
			WindowAction *aa = a->FindAction(s->e[c2]->action);
			bool display = s->e[c2]->info1;

			//(gtk_accel_path "<Actions>/image/image-scale" "<Primary><Alt>o")

			LaxToXlibKey(keyname, s->e[c2]->keys->key);

			scratch = aa->description;
			scratch.Replace("", " ", true);
			fprintf(f,"  <bind action=\"%s\" key=\"%s\" modifiers=\"", scratch.c_str(), keyname);

			int n=0;
			if (s->e[c2]->keys->state & ShiftMask)   { n++; fputs("Shift", f); }
			if (s->e[c2]->keys->state & ControlMask) { n++; fprintf(f, "%sCtrl", n>1 ? "," : ""); }
			if (s->e[c2]->keys->state & AltMask)     { n++; fprintf(f, "%sAlt",  n>1 ? "," : ""); }
			if (s->e[c2]->keys->state & MetaMask)    { n++; fprintf(f, "%sMeta", n>1 ? "," : ""); }

			fprintf(f, "\" " );
			if (display) fputs("display=\"yes\"", f);
			fputs("/>\n", f);
		}
		 
//		//write out all unbound actions: ...unnecessary?
//		for (int c2=0; c2<a->n; c2++) {
//			WindowAction *aa = a->e[c2];
//			if (s->FindShortcutFromAction(aa->id,0)) continue; //already output above
//
//			fprintf(f,"; (gtk_accel_path \"%s\" \"\")\n", aa->description);
//		}
	}

	fputs("</keys>\n", f);
	fclose(f);
	return 0;
}


//----------------------------- Krita -------------------------

int ImportKrita(const char *file)
{
	FILE *f = fopen(file, "r");
	if (!f) return 1;

	char *str, *endptr;
	char *desc, *key;
	char *line = nullptr;
	size_t n = 0;
	ssize_t c;
	int actionnumber = 0;
	unsigned int ch;
	unsigned int mods;

	ShortcutManager *manager = GetDefaultShortcutManager();
	makestr(manager->setfile, file);
	makestr(manager->setname, "Krita");
	Laxkit::ShortcutHandler *handler = new Laxkit::ShortcutHandler("Krita"); 

	while (!feof(f)) {
		c = getline(&line, &n, f);
		if (c <= 0) break;

		if (strstr(line, "[Shortcuts]")) continue;

		//Select Behind Blending Mode=Alt+Shift+Q

		//get description
		str = strstr(line, "=");
		if (!str) continue;

		desc = newnstr(line, str-line);
		str++;
		
		//get key + mods
		unsigned int ch = 0;
		unsigned int mods = 0;

		while (1) {
			if (strstr(str, "Ctrl+"))  { str += 5; mods |= ControlMask; }
			if (strstr(str, "Alt+"))   { str += 4; mods |= AltMask; }
			if (strstr(str, "Shift+")) { str += 6; mods |= ShiftMask; }
			if (strstr(str, "Meta+"))  { str += 5; mods |= MetaMask; }
			break;
		}

		if (!strcmp(str, "\\\\")) ch = '\\';
		else {
			// *** alts may be as : "Alt+[; Shift+space"
			stripws(str, 2);
			const char *nstr = XlibToNormal(str);
			ch = key_value_from_name(nstr);
			if (ch >= 'A' && ch <= 'Z' && (mods & ShiftMask) == 0) ch -= 'A'-'a';
			if (ch >= 'a' && ch <= 'z' && (mods & ShiftMask) != 0) ch += 'A'-'a';
		}

		if (ch != 0) {
			handler->Add(actionnumber++, ch,mods,0, desc, desc,NULL,0);
		} else {
			handler->AddAction(actionnumber++, desc, desc, nullptr, 0,0);
		}

		delete[] desc;
	}

	free(line);

	fclose(f);

	manager->AddArea("Krita",handler);
	//manager->dump_out(stdout, 0, 0, nullptr);
	return 0;
}

int ExportKrita(const char *file)
{
	DBG cerr << "Export Krita..."<<endl;

	ShortcutManager *manager = GetDefaultShortcutManager();
	if (manager->shortcuts.n == 0) return 1;

	FILE *f = fopen(file, "w");
	if (!f) return 1;

	fputs("[Shortcuts]\n", f);

	char keyname[100];

	if (manager->shortcuts.n > 1) {
		cerr << "Warning!! Exporting to Krita key format, but more than one shortcut area!" << endl;
	}

	for (int c=0; c<manager->shortcuts.n; c++) {
		ShortcutDefs  *s = manager->shortcuts.e[c]->Shortcuts();
	    WindowActions *a = manager->shortcuts.e[c]->Actions();

		 //output all bound keys. Each physical key gets a list of action+mods
		if (!s || !a) continue;


		//all bound keys:
		for (int c2=0; c2<s->n; c2++) {
			KeyInfo * ki = s->e[c2]->keys;
			WindowAction *aa = a->FindAction(s->e[c2]->action);

			//(gtk_accel_path "<Actions>/image/image-scale" "<Primary><Alt>o")

			LaxToXlibKey(keyname, s->e[c2]->keys->key);

			fprintf(f,"%s=", aa->description);

			if (s->e[c2]->keys->state & ShiftMask)   { fputs("Shift+",f); }
			if (s->e[c2]->keys->state & ControlMask) { fputs("Ctrl+", f); }
			if (s->e[c2]->keys->state & AltMask)     { fputs("Alt+",  f); }
			if (s->e[c2]->keys->state & MetaMask)    { fputs("Meta+", f); }

			fprintf(f, "%s\n", keyname);
		}
		 
//		//write out all unbound actions: ...unnecessary?
//		for (int c2=0; c2<a->n; c2++) {
//			WindowAction *aa = a->e[c2];
//			if (s->FindShortcutFromAction(aa->id,0)) continue; //already output above
//
//			fprintf(f,"; (gtk_accel_path \"%s\" \"\")\n", aa->description);
//		}
	}

	fclose(f);
	return 0;
}

//----------------------------- Gimp -------------------------

unsigned int GimpToLax(const char *k)
{
	k = XlibToNormal(k);
	return key_value_from_name(k);

//	if (!k || *k==0) return 0;
//
//	if (k[1]=='\0' && k[0]>='A' && k[0]<='Z') return 'a' + (int)k[0] - 'A'; //ascii printable
//	return 0;
}

int ImportGimp(const char *file)
{
	FILE *f = fopen(file, "r");
	if (!f) return 1;

	char *str, *endptr;
	char *desc, *key;
	char *line = nullptr;
	size_t n = 0;
	ssize_t c;
	int actionnumber = 0;
	unsigned int ch;
	unsigned int mods;

	ShortcutManager *manager = GetDefaultShortcutManager();
	makestr(manager->setfile, file);
	makestr(manager->setname, "Gimp");
	Laxkit::ShortcutHandler *handler = new Laxkit::ShortcutHandler("Gimp"); 

	while (!feof(f)) {
		c = getline(&line, &n, f);
		if (c <= 0) break;

		//; (gtk_accel_path "<Actions>/file/file-save-as" "<Primary><Shift>s")

		str = strstr(line, "gtk_accel_path");
		if (!str) continue;

		str = str + 15; 

		//get description
		desc = QuotedAttribute(str, &endptr);
		if (isblank(desc)) continue;
		str = endptr;
		
		//get key + mods
		key = QuotedAttribute(str, &endptr);
		if (!key) { delete[] desc; continue; }

		ch = 0;
		mods = 0;
		if (strstr(key, "<Primary>")) mods |= ControlMask;
		if (strstr(key, "<Alt>"))     mods |= AltMask;
		if (strstr(key, "<Shift>"))   mods |= ShiftMask;

		str = key;
		endptr = strstr(str, ">");
		while (endptr) {
			str = endptr+1;
			endptr = strstr(str, ">");
		}
		if (!isblank(str)) {
			ch = GimpToLax(str);
			if (ch == 0) cerr << " WARNING! couldn't find gimp key "<<key<<endl;
		}

		// *** should uppercase 1st letter, and turn - to spaces
		char *bname = newstr(lax_basename(desc));
		for (int cc=0; cc<strlen(bname); cc++) {
			if (cc == 0) bname[cc] = toupper(bname[cc]);
			if (bname[cc] == '-') {
				bname[cc] = ' ';
				bname[cc+1] = toupper(bname[cc+1]);
				cc++;
			}
		}

		if (ch != 0) {
			handler->Add(actionnumber++, ch,mods,0, desc, bname, NULL,0);
		} else {
			handler->AddAction(actionnumber++, desc, bname, nullptr, 0,0);
		}

		delete[] bname;
		delete[] desc;
		delete[] key;
	}

	free(line);

	fclose(f);

	manager->AddArea("Gimp",handler);
	//manager->dump_out(stdout, 0, 0, nullptr);
	return 0;
}

int ExportGimp(const char *file)
{
	DBG cerr << "Export Gimp..."<<endl;

	ShortcutManager *manager = GetDefaultShortcutManager();
	if (manager->shortcuts.n == 0) return 1;

	FILE *f = fopen(file, "w");
	if (!f) return 1;

	fputs("; gimp GtkAccelMap rc-file         -*- scheme -*-\n"
		   "; this file is a dump from Shortcutout\n"
		   ";\n", f);

	char keyname[100];

	if (manager->shortcuts.n > 1) {
		cerr << "Warning!! Exporting to gimp key format, but more than one shortcut area!" << endl;
	}

	for (int c=0; c<manager->shortcuts.n; c++) {
		ShortcutDefs  *s = manager->shortcuts.e[c]->Shortcuts();
	    WindowActions *a = manager->shortcuts.e[c]->Actions();

		 //output all bound keys. Each physical key gets a list of action+mods
		if (!s || !a) continue;


		//all bound keys:
		for (int c2=0; c2<s->n; c2++) {
			KeyInfo * ki = s->e[c2]->keys;
			WindowAction *aa = a->FindAction(s->e[c2]->action);

			//(gtk_accel_path "<Actions>/image/image-scale" "<Primary><Alt>o")

			LaxToXlibKey(keyname, s->e[c2]->keys->key);

			fprintf(f,"(gtk_accel_path \"%s\" \"", aa->description);

			if (s->e[c2]->keys->state & ShiftMask)   { fputs("<Shift>",   f); }
			if (s->e[c2]->keys->state & ControlMask) { fputs("<Primary>", f); }
			if (s->e[c2]->keys->state & AltMask)     { fputs("<Alt>",     f); }
			if (s->e[c2]->keys->state & MetaMask)    { fputs("<Super>",   f); }

			fprintf(f, "%s\")\n", keyname);
		}
		 
		//write out all unbound actions: ...unnecessary?
		for (int c2=0; c2<a->n; c2++) {
			WindowAction *aa = a->e[c2];
			if (s->FindShortcutFromAction(aa->id,0)) continue; //already output above

			fprintf(f,"; (gtk_accel_path \"%s\" \"\")\n", aa->description);
		}
	}

	fclose(f);
	return 0;
}


//------------------------------------------------------
class ShortcutWin : public ShortcutWindow2
{

  public:
	enum ExtraActions {
		EXPORT_ShortcutMapper = KMENU_MAX + 1,
		EXPORT_Gimp,
		EXPORT_Krita,
		EXPORT_Darktable,
		EXPORT_Inkscape,
		EXPORT_Scribus,
		EXPORT_Laidout,
		EXPORT_Godot,
		EXPORT_Blender,
	};
	
	ShortcutWin(const char *context)
     : ShortcutWindow2(nullptr,"kb","kb", ANXWIN_ESCAPABLE,
											0,200,1500,800,0,
											context)
	{}

	Laxkit::MenuInfo *GetSettingsMenu();
	int Event(const Laxkit::EventData *e,const char *mes);

};

MenuInfo *ShortcutWin::GetSettingsMenu()
{
	MenuInfo *menu = ShortcutWindow2::GetSettingsMenu();

	int i = menu->findIndex(_("Keyboards"));

	menu->AddItem(_("Export ShortcutMapper"), EXPORT_ShortcutMapper, i++);
	menu->AddItem(_("Export Gimp"),           EXPORT_Gimp,           i++);
	menu->AddItem(_("Export Krita"),          EXPORT_Krita,          i++);
	menu->AddItem(_("Export Inkscape"),       EXPORT_Inkscape,       i++);
	menu->AddItem(_("Export Scribus"),        EXPORT_Scribus,        i++);
	//menu->AddItem(_("Export Laidout"),        EXPORT_Laidout,        i++);
	//menu->AddItem(_("Export Godot"),          EXPORT_Godot,          i++);
	//menu->AddItem(_("Export Darktable"),      EXPORT_Darktable,      i++);
	//menu->AddItem(_("Export Blender"),        EXPORT_Blender,        i++);


	return menu;
}

int ShortcutWin::Event(const Laxkit::EventData *e,const char *mes)
{
	if (!strcmp(mes,"settingsm")) {
		const SimpleMessage *s = dynamic_cast<const SimpleMessage*>(e);
		if (!s) return 1;

		ShortcutManager *manager = GetDefaultShortcutManager();
		const char *defaultfile = manager->setfile;
		//if (isblank(defaultfile)) defaultfile = "keys";
		const char *what = nullptr;

		if (s->info2 == EXPORT_ShortcutMapper) what = "ShortcutMapper"; 
		else if (s->info2 == EXPORT_Gimp     ) what = "Gimp"; 
		else if (s->info2 == EXPORT_Krita    ) what = "Krita"; 
		else if (s->info2 == EXPORT_Inkscape ) what = "Inkscape"; 
		else if (s->info2 == EXPORT_Scribus  ) what = "Scribus"; 
		else if (s->info2 == EXPORT_Laidout  ) what = "Laidout"; 
		else if (s->info2 == EXPORT_Godot    ) what = "Godot"; 
		else if (s->info2 == EXPORT_Blender  ) what = "Blender"; 
		else if (s->info2 == EXPORT_Darktable) what = "Darktable"; 

		if (what) {
			Utf8String exportWhat(_("Export for %s..."), what);
			FileDialog *f = new FileDialog(nullptr, what, exportWhat.c_str(), ANXWIN_REMEMBER,
									0,0,800,500,0,object_id, what,
									FILES_SAVE | FILES_ASK_TO_OVERWRITE,
                                    defaultfile);
            app->rundialog(f);
            return 0;

		} else {
			return ShortcutWindow2::Event(e, mes);
		}

	} else if (!strcmp(mes, "ShortcutMapper")) {
		const SimpleMessage *s = dynamic_cast<const SimpleMessage*>(e);
		if (!s) return 0;
		ExportShortcutMapper(keyboard->GetKeyboard(), s->str);
		return 0;

	} else if (!strcmp(mes, "Gimp")) {
		const SimpleMessage *s = dynamic_cast<const SimpleMessage*>(e);
		if (!s) return 0;
		ExportGimp(s->str);
		return 0;

	} else if (!strcmp(mes, "Scribus")) {
		const SimpleMessage *s = dynamic_cast<const SimpleMessage*>(e);
		if (!s) return 0;
		ExportScribus(s->str);
		return 0;

	} else if (!strcmp(mes, "Krita")) {
		const SimpleMessage *s = dynamic_cast<const SimpleMessage*>(e);
		if (!s) return 0;
		ExportKrita(s->str);
		return 0;

	} else if (!strcmp(mes, "Inkscape")) {
		const SimpleMessage *s = dynamic_cast<const SimpleMessage*>(e);
		if (!s) return 0;
		ExportInkscape(s->str);
		return 0;
	}

	return ShortcutWindow2::Event(e, mes);
}

//------------------------------------------------------

int main(int argc,char **argv)
{
	anXApp app;
	app.init(argc,argv);


	const char *file = "KeyFormats/default.keys";
	if (argc > 1) file = argv[1];



	// *** parse keys maybe from:
	//  blender
	//  gimp
	//  inkscape
	//  scribus
	//  darktable
	//  krita
	//  godot

	char first1000[1000];
	FILE *f = fopen(file, "r");
	if (!f) {
		cerr << "usage: ./shortcuts file"<<endl;
		exit(1);
	}

	size_t c = fread(first1000, 1, 999, f);
	first1000[c] = '\0';
	fclose(f);

	const char *default_scribus  = "~/.config/";
	const char *default_gimp     = "~/.config/";
	const char *default_inkscape = "~/.config/";
	const char *default_krita    = "~/.config/";
	const char *default_laidout  = "~/.config/laidout/0.097.1/default.keys";
	//const char *default_blender  = "~/.config/";
	//const char *default_godot    = "~/.config/godot/editor_settings-3.tres";


	string default_context;
	string original_format;

	if (strstr(first1000, "Scribus")) {
		// Scribus!
		ImportScribus(file);
		original_format = "scribus";

	} else if (strstr(first1000, "gimp GtkAccelMap")) {
		//gimp!
		ImportGimp(file);
		original_format = "gimp";

	} else if (strstr(first1000, "Inkscape")) {
		//inkscape!
		ImportInkscape(file);
		original_format = "inkscape";

	} else if (strstr(first1000, "[Shortcuts]") && strstr(first1000, "BrushesAndStuff")) {
		//Krita!
		ImportKrita(file);
		original_format = "krita";

	} else if (first1000[0] == '{' && strstr(first1000, "\"contexts\"")) {
		//ShortcutMapper file!
		ImportShortcutMapper(file, default_context);
		original_format = "ShortcutMapper";

//	} else if (strstr(first1000, "import bpy")) {
//		// blender!
//		cerr << " *** need to implement blender import!!"<<endl;
//		exit(1);
//
//	} else if (strstr(first1000, "[gd_resource type=\"EditorSettings")) {
//		//Godot!
//		cerr << " *** need to implement godot import!!"<<endl;
//		exit(1);
//		original_format = "Godot";
	}


    ShortcutWin *kb = new ShortcutWin(default_context.c_str());
    kb->ApplyCurrentLocale();

	app.addwindow(kb);

	app.run();
	return app.close();
}
