#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>

struct XmlSnakeReader{
	int depth = -1;
	int nodeType = -1;
	string name = "";
	bool empty = true;
	bool hasValue = false;
	size_t valueLen = 0;
	string value = "";
	size_t attributeLen = 0;
	string *attributes = NULL;
};
class XmlSnake{
	private:
	xmlTextWriterPtr writer = NULL;
	xmlTextReaderPtr reader = NULL;
	xmlBufferPtr writer_output = NULL;

	string encodingVersion = "ISO-8859-1";
	int readerDescriptor = -1;

	public:
	struct XmlSnakeReader readResult;

	/*
	 * Writer Functions
	 * */
	bool writeComment(string comment){
		//xmlChar *tmp = ConvertInput(comment.c_str(), encodingVersion.c_str());
		if(xmlTextWriterWriteComment(writer, (const xmlChar*)comment.c_str()) < 0){
			return false;
		}
		return true;
	}
	bool writeAttribute(string name, string value){
		if(xmlTextWriterWriteAttribute(writer, (const xmlChar*)name.c_str(), (const xmlChar*)value.c_str()) < 0){
			return false;
		}
		return true;
	}
	bool openFileWriter(string fileName){
		writer = xmlNewTextWriterFilename(fileName.c_str(), 0);
		if(writer == NULL){
			return false;
		}
		return true;
	}

	bool openStringWriter(void){
		writer_output = xmlBufferCreate();
		writer = xmlNewTextWriterMemory(writer_output, 0);
		if(writer == NULL){
			return false;
		}
		return true;
	}

	bool startWritingFile(void){
		if(xmlTextWriterStartDocument(writer, NULL, encodingVersion.c_str(), NULL) < 0){
			return false;
		}
		return true;
	}

	bool startWritingString(void){
                if(xmlTextWriterStartDocument(writer, NULL, encodingVersion.c_str(), NULL) < 0){
                        return false;
                }
                return true;
        }

	bool startWritingElement(string name){
		if(xmlTextWriterStartElement(writer, (const xmlChar*)name.c_str()) < 0){
			return false;
		}
		return true;
	}
	bool writeElement(string name, string value){
		if(xmlTextWriterWriteElement(writer, (const xmlChar*)name.c_str(), (const xmlChar*)value.c_str())){
			return false;
		}
		return true;
	}

	bool stopWritingElement(void){
		if(xmlTextWriterEndElement(writer) < 0){
			return false;
		}
		return true;
	}

	bool stopWritingFile(void){
		if(xmlTextWriterEndDocument(writer) < 0){
			return false;
		}
		return true;
	}

	void closeFileWriter(void){
		if(writer != NULL){
			xmlFreeTextWriter(writer);
			writer = NULL;
		}
	}

	void closeWriter(){
		closeFileWriter();
	}

	void freeWriterOutput(){
		if(writer_output != NULL){
			xmlBufferFree(writer_output);
			writer_output = NULL;
		}
	}

	string getCreatedString(){
		if(writer_output == NULL){
			return "";
		}
		string ret = (const char *)writer_output->content;
		return ret;
	}
	/*
	 * Reader functions
	 * */
	bool openFileReader(string fileName){
		reader = xmlReaderForFile(fileName.c_str(), NULL, 0);
		if(reader == NULL){
			return false;
		}
		return true;
	}

	bool openStringReader(string content){
		size_t size = content.length();
		char *data = new char[size];
		for(int i=0; i<size; i++){
			data[i] = content[i];
		}
		reader = xmlReaderForMemory(data, (int)size, NULL, NULL, 0);
		if(reader == NULL){
			return false;
		}
		return true;
	}

	bool readLineReader(void){
		readerDescriptor = xmlTextReaderRead(reader);
		if(readerDescriptor != 1)
			return false;
		processLine();
		return true;
	}

	void processLine(void){
		const xmlChar *name;
		const xmlChar *value;
		name = xmlTextReaderConstName(reader);
		value = xmlTextReaderConstValue(reader);
		readResult.depth = xmlTextReaderDepth(reader);
	        readResult.nodeType = xmlTextReaderNodeType(reader);
		if(name == NULL)
			readResult.name = "";
		else
			readResult.name = (const char *)name;
	        readResult.empty = xmlTextReaderIsEmptyElement(reader) == 1 ? true : false;
	        readResult.hasValue = xmlTextReaderHasValue(reader) == 1 ? true : false;
	        readResult.valueLen = xmlStrlen(value);
		if(value == NULL)
	        	readResult.value = "";
		else
			readResult.value = (const char *)value;
		readResult.attributeLen = xmlTextReaderAttributeCount(reader);
		if(readResult.attributeLen > 0){
			if(readResult.attributes != NULL)
				delete[] readResult.attributes;
			readResult.attributes = new string[readResult.attributeLen];
			for(int i=0; i<readResult.attributeLen; i++){
				readResult.attributes[i] = (const char *)xmlTextReaderGetAttributeNo(reader, i);
			}
		}else if(readResult.attributes != NULL){
			delete[] readResult.attributes;
			readResult.attributes = NULL;
		}
	}

	string getAttributeByName(string name){
		xmlChar * val = xmlTextReaderGetAttribute(reader, (const xmlChar*)name.c_str());
		if(val == NULL)
			return "";
		string ret = (const char *)val;
		return ret;
	}

	void closeReader(){
		if(reader != NULL){
			xmlFreeTextReader(reader);
			xmlCleanupParser();
			reader = NULL;
		}
	}
	void cleanReader(){
		xmlCleanupParser();
	}
};
