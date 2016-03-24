/*****************************************************

	xmlDocument.cpp
		XML Document API

*****************************************************/

#include "xml.h"
using namespace std;

namespace hgxml {

	/*
		XML Element
	*/

	elementXML::~elementXML() {
		std::vector< elementXML* >::iterator it = children.begin();
		for (; it!=children.end(); ++it)
			delete *it;
		children.clear();
	}

	void elementXML::addChild (elementXML* e) {
		if(e) {
			e->setParent (this);
			children.push_back(e);
		}
	}

	void elementXML::removeChild ( elementXML* e ) {
		std::vector< elementXML* >::iterator it = std::find(children.begin(), children.end(), e);
		if (it != children.end())
			children.erase(it);
	}

	void elementXML::addAttribute (attributeXML e) {
		attributes.push_back(e);
	}

	void elementXML::addAttribute (std::string& key, std::string& value) {
		addAttribute (attributeXML (key,value));
	}

	void elementXML::setAttribute (std::string& key, std::string& value) {
		std::vector< attributeXML >::iterator it = attributes.begin();
		for (; it!=attributes.end(); it++) {
			if (it != attributes.end()) {
				(*it).second = value;
				break;
			}
		}
	}

	std::string elementXML::getAttribute (std::string& key) {
		std::vector< attributeXML >::iterator it = attributes.begin();
		for (; it!=attributes.end(); it++) {
			if ((*it).first == key) {
				return (*it).second;
			}
		}
		return "";
	}

	void elementXML::removeAttribute (std::string key) {
		std::vector< attributeXML >::iterator it = attributes.begin();
		for (; it!=attributes.end(); it++) {
			if ((*it).first == key) {
				attributes.erase(it);
				break;
			}
		}
	}

	elementXML* elementXML::createChild () {
		elementXML* child = new elementXML();
		addChild (child);
		return child;
	}

	elementXML* elementXML::createChild (std::string& name, elementXML* parent, elementTypeXML type, std::string& content) {
		elementXML* child = new elementXML(name, parent, type, content);
		addChild (child);
		return child;
	}

	void elementXML::removeAttribute ( attributeXML e ) {
		removeAttribute (e.first);
	}

	ostream& elementXML::serialize (ostream& out, std::string spaces) {
		//
		// Write tag start
		//
		string close = "";
		out << spaces;
		switch (type) {
			case elementTypeXML::Comment: {
				out << "<!-- " << content << " -->" << endl;
				return out;
			}
			case elementTypeXML::Declaration: {
				out << "<?" << name;
				close = "";
				break;
			}
			case elementTypeXML::Standard:
			default: {
				out << "<" << name;
				close = "</" + name + ">";
			}
		}
		//
		// save attributes
		//
		if (attributes.size() > 1) {
			for (size_t i=0; i<attributes.size(); i++) {
				cout << " \"" << attributes[i].first << "\"" << "=\"" << attributes[i].second << "\"" << endl;
				out << " " << attributes[i].first << "=\"" << attributes[i].second << "\"";
			}
		}
		//
		// test if this is a self closing tag (no children or is root)
		//
		if (parent && children.size()==0) {
			out << "/>" << endl;
			return out;
		}
		if (!parent)
			out << "?>" << endl;
		else
			out << ">" << endl;
		//
		// save children
		//
		for (size_t i=0; i<children.size(); i++)
			children[i]->serialize (out, spaces + "    ");
		out << spaces << close << endl;
		return out;
	}

	std::vector<std::string> tokenize(const std::string & str, const std::string & delim)
	{
	  using namespace std;
	  vector<string> tokens;

	  size_t p0 = 0, p1 = string::npos;
	  while(p0 != string::npos)
	  {
		p1 = str.find_first_of(delim, p0);
		if(p1 != p0)
		{
		  string token = str.substr(p0, p1 - p0);
		  tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	  }

	  return tokens;
	}

	/*
		XML Document API
	*/

	documentXML::documentXML () : root(0) {
	}

	documentXML::~documentXML () {
		//release ();
	}

	void documentXML::release () {
		delete root;
	}

	void documentXML::parseTag (elementXML*& currentElement, std::string data) {

		if (!currentElement) {
			currentElement = new elementXML ();
			if (!root) {
				root = currentElement;
			}
		}
		else {
			elementXML* child = currentElement->createChild ();
			if (child)
				currentElement = child;
		}

		size_t start=0, end=0;
		bool close=false;

		currentElement->content = data;
		switch (data[1]) {
			case '!': {
				//
				// Comment syntax: <!-- ... -->
				//
				cout << "CMT";
				currentElement->type = elementTypeXML::Comment;
				currentElement->content = data;
				close=true;
				break;
			}
			case '?': {
				//
				// Declaration syntax:  <?NAME ARG1="VALUE" ... ?>
				//
				start = 2;
				end = data.find_first_of (" \t", start);
				currentElement->type = elementTypeXML::Declaration;
				// includes last space?
				currentElement->name = data.substr(start,end-1);
				cout << "DOCNAME: '" << currentElement->name << "'" << endl;
				break;
			}
			case '/': {
				//
				// Close tag syntax: </NAME [optional space] >
				// We dont add these to the tree. Just remove the child
				// and go to its parent parent. Name must match parent there is a tag mismatch
				//
				elementXML* child = currentElement;
				currentElement->getParent()->removeChild (child);
				currentElement = currentElement->getParent()->getParent();
				delete child;
				return;
			}
			default:
				//
				// Open tag syntax: <NAME ARG1="VALUE" ... >
				//
				start = 1;
				end = data.find_first_of (" \t", start);
				currentElement->type = elementTypeXML::Standard;
				// includes last space?
				currentElement->name = data.substr(start,end-1);
				cout << "NAME: '" << currentElement->name << "'" << endl;
				break;
		}

		// attributes
		while (true) {

			start = data.find_first_not_of (" \t", end+1);
			end = data.find_first_of ("=", start+1);
			if (end==data.npos)
				break;
			attributeXML attrib;
			attrib.first = data.substr (start,end-start);

			start = end+1;
			start = data.find_first_not_of(" \t", start);
			end = data.find_first_of ("\"", start+1);
			attrib.second = data.substr (start+1, end-start-1);

			currentElement->addAttribute (attrib);
		}

		start = data.length()-2;
		if (data[start]=='/' && data[start+1]=='>')
			close=true;

		if (close)
			currentElement = currentElement->getParent();

		cout << "\t" << data << endl;
	}

	bool documentXML::open (std::string& fname) {
		//
		// Load file into string
		//
		ifstream file(fname.c_str());
		if (!file.is_open()) {
			cout << "Error";
			return false;
		}
		cout << "XML LOADED" << endl;
		string  buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		//
		// Split buffer into elements
		//
		vector<string> elements;
		size_t prev = 0, pos = 0;
		while ((pos = buffer.find_first_of("<", prev)) != string::npos) {
			string element;
			size_t end = 0;
			if (buffer[pos+1]=='!' && buffer[pos+2]=='-' && buffer[pos+3]=='-') {
				end = buffer.find("-->", pos+4);
				element = buffer.substr(pos,end+4-pos);
				elements.push_back(element);
				prev=pos+element.size();
			}
			else {
				end = buffer.find_first_of(">", pos+1);
				element = buffer.substr(pos,end+1-pos);
				elements.push_back(element);
				prev=pos+1;
			}
			//
			// get data field length
			//
			size_t data_end = buffer.find_first_of("<", prev);
			size_t data_start = end+1;
			//
			// store data field for this element
			//
			string data = buffer.substr(data_start,data_end-data_start);
			elements.push_back(data);
		}
		//
		// Release buffer and file memory
		//
		buffer.clear();
		file.close();
		//
		// Build element tree
		//
		elementXML* currentElement = 0;
		for (size_t current = 0; current < elements.size(); current++) {
			/* this is a tag element */
			if (elements[current][0]=='<') {
				parseTag (currentElement, elements[current]);
			}
			else {
				/*
					setData appends to current element so this handles cases like
					<TAG>MyData <NEWTAG> MyOtherData </NEWTAG> MoreData</TAG>
					resulting in <TAG> data = 'MyData MoreData' and NEWTAG data = 'MyOtherData'
				*/
				if (currentElement)
					currentElement->setData (elements[current]);
			}
		}
		return true;
	}

	bool documentXML::open (std::wstring& fname) {
		return false;
	}

	bool documentXML::save (std::string& fname) {
		ofstream file(fname.c_str());
		if (!file.is_open()) {
			cout << "Error";
			return false;
		}
		getRoot()->serialize (file);
		file.close();
		return false;
	}

	bool documentXML::save (std::wstring& fname) {
		return false;
	}
}
