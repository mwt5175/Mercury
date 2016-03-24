/*****************************************************

	xmlDocument.h
		XML Document API

*****************************************************/

#ifndef XML_H
#define XML_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace hgxml {
	//
	// Element types can be comments <!-- .. -->, declaration <? .. ?>,
	// or Standard < ... >, < ... />, </ ... >
	//
	struct elementTypeXML {
		enum Enum {
			Comment,
			Declaration,
			Standard,
			MAX
		};
		elementTypeXML (Enum value = MAX) : value(value) {}
		friend bool operator == (elementTypeXML const& a, elementTypeXML const& b) {
			return a.value == b.value;
		}
		friend bool operator != (elementTypeXML const& a, elementTypeXML const& b) {
			return a.value != b.value;
		}
		operator int() {
			return value;
		}
	private:
		Enum value;
	};

	struct elementErrorXML {
		enum Enum {
				OpenFileFail,
				CreateFileFail,
				IncorrectContent,
				NotClosedElements,
				NoXMLDeclaration,
				CloseNotOpenedElement,
				InconsistentOpenCloseElements,
				MoreThanOneXMLDeclaration,
				MoreThanOneRootElement,
				IncorrectAttribute,
				MAX
		};

		elementErrorXML (Enum e) {value=e;}
		Enum getError () {return value;}

	private:
		Enum value;
	};

	//
	// An attribute is a name=value pair
	//
	typedef std::pair <std::string, std::string> attributeXML;

	//
	// XML elements can contain other elements, text and attributes
	//
	class elementXML {

		friend class documentXML;

		std::string    name;
		std::string    content;
		std::string    data;
		elementTypeXML type;
		elementXML*    parent;
		std::vector < elementXML* >  children;
		std::vector < attributeXML > attributes;

		std::ostream& serialize (std::ostream&, std::string="");

	public:
		elementXML () : parent (0), type (elementTypeXML::MAX) {}
		elementXML (std::string& name, elementXML* parent, elementTypeXML type, std::string& content)
			: name(name), parent(parent), type(type), content(content) {}
		virtual ~elementXML ();

		void        setName (std::string& n)    { name=n; }
		std::string getName ()                  { return name;}
		void        setContent (std::string& n) { content=n; }
		std::string getContent ()               { return content;}
		void        addContent (std::string& n) { content+=n;}
		void        setType (elementTypeXML n)  { type=n; }
		elementTypeXML getType ()               { return type; }
		void        setParent (elementXML* e)   { parent=e; }
		elementXML* getParent ()                { return parent; }
		std::string getData ()                  { return data; }
		void        setData (std::string& n)    { data+=n; }

		elementXML* createChild ();
		elementXML* createChild  (std::string& name, elementXML* parent, elementTypeXML type, std::string& content);

		void addChild            (elementXML* e);
		void removeChild         (elementXML* e );
		void addAttribute        (attributeXML e);
		void addAttribute        (std::string& key, std::string& value);
		void setAttribute        (std::string& key, std::string& value);
		std::string getAttribute (std::string& key);
		void removeAttribute     (std::string key);
		void removeAttribute     (attributeXML e );
	};

	//
	// XML document API
	//
	class documentXML {
		elementXML* root;
		elementXML* declarator;

		void parseTag (elementXML*&, std::string);

	public:
		documentXML ();
		virtual ~documentXML ();
		void release ();
		/* opens and parses an XML file */
		bool open (std::string& fname);
		bool open (std::wstring& fname);

		/* saves element tree to XML file */
		bool save (std::string& fname);
		bool save (std::wstring& fname);

		/* returns root element */
		elementXML* getRoot() {return root;}
		elementXML* getDeclarator() {return declarator;}
	};
}

#endif
