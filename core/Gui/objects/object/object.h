class CoreObject{
	private:
		int objectId = -1;
                string objectName = "";
                float constraintX = 0;
                float constraintY = 0;
                float constraintW = 0;
                float constraintH = 0;
                GLuint texture = 0;
                int displayX = 0;
                int displayY = 0;

                void getDisplayResolution(void){
                        if(this->displayX == 0 && this->displayY == 0){
                                Display* d = XOpenDisplay(NULL);
                                Screen*  s = DefaultScreenOfDisplay(d);
                                this->displayX = s->width;
                                this->displayY = s->height;
                                XCloseDisplay(d);
                        }
                }
	public:
		/*
		 * Setters and Getters.
		 * */
		void setConstraints(float x, float y, float w, float h){
			this->constraintX = this->getX(x);
			this->constraintY = this->getY(y);
			this->constraintW = w;
			this->constraintH = h;
		}
		void setObjectId(int id){
			this->objectId = id;
		}
		int getObjectId(void){
			return this->objectId;
		}
		void setObjectName(string name){
			this->objectName = name;
		}
		string getObjectName(void){
			return this->objectName;
		}
		void setConstaintX(float x){
			this->constraintX = x;
		}
		float getConstraintX(){
			return this->constraintX;	
		}
		void setConstraintY(float y){
			this->constraintY = y;
		}
		float getConstraintY(){
			return this->constraintY;
		}
		void setConstraintW(float w){
			this->constraintW = w;
		}
		float getConstraintW(void){
			return this->constraintW;
		}
		void setConstraintH(float h){
			this->constraintH = h;
		}
		float getConstraintH(void){
			return this->constraintH;
		}

		/*
		 * Configuration Functions.
		 * */

		float getXCenter(void){
			this->getDisplayResolution();
			return getX(this->displayX/2);
		}
		float getX(float x){
			this->getDisplayResolution();
                        if(x >=0 && x < this->displayX/2){
                                x = -1 + (x*((9999999/(this->displayX/2))*0.0000001));
                        }else if(x == this->displayY/2){
                                x = 0;
                        }else if(x > this->displayX/2 && x < this->displayX){
                                x = -1 + (x*((9999999/(this->displayX/2))*0.0000001));
                        }else if(x == this->displayX){
                                x = 1;
                        }
                        return x;
                }

                float getMultiple(float s, float p){
                        return s / 1000;
                }
                float getContainedX(float x){
                        x = this->getX(x) + this->constraintX;
                        return x;
                }
		
		float getYCenter(){
			this->getDisplayResolution();
			return getY(this->displayY/2);
		}
                float getY(float y){
			this->getDisplayResolution();
                        if(y >=0 && y < this->displayY/2){
                                y = -1 + (y*((9999999/(this->displayY/2))*0.0000001));
                        }else if(y == this->displayY/2){
                                y = 0;
                        }else if(y > this->displayY/2 && y < this->displayY){
                                y = -1 + (y*((9999999/(this->displayY/2))*0.0000001));
                        }else if(y == this->displayY){
                                y = 1;
                        }
                        return y;
                }

		float getContainedY(float y){
			y = this->getY(y) + this->constraintY;
			return y;
		}
		/*
		 * Virtual Drawing Functions 
		 * */
		virtual void drawContainedTextArray(float x, float y, float z, float w, float h, string text, void* bitmap, float spacing, int maxXOverflow){
                        if(text.empty())
                                return;

                        bool overFlowed = false;
                        size_t textArraySize = text.length();
                        if(textArraySize > maxXOverflow){
                                textArraySize = maxXOverflow + 3;
                                overFlowed = true;
                        }

                        string *textArray = new string[textArraySize];

                        for(int i=0; i<textArraySize; i++){
                                if(i < maxXOverflow)
                                        textArray[i] = text[i];
                                else{
                                        textArray[i] = ".";
                                }
                        }

                        for(int i=0; i<textArraySize; i++){
                                if(i != 0)
                                        x += spacing;
                                this->drawContainedText(x, y, z, textArray[i], bitmap);
                        }

                }

		virtual void startDrawing(){
                        glBegin(GL_POLYGON);
                }

                virtual void stopDrawing(){
                        glEnd();
                        glFlush();
                }

		virtual void setColor(float r, float g, float b){
                        glColor3f(r, g, b);
                }

                virtual void setVertex(float x, float y, float z){
                        glVertex3f(x, y, z);
                }

		virtual void drawText(float x, float y, float z, string text, void* bitmap){
			glRasterPos3f(x, y, z);
			glutBitmapString(bitmap, (const unsigned char *)text.c_str());
		}

		virtual void drawContainedText(float x, float y, float z, string text, void* bitmap){
			x = this->getX(this->getContainedX(x));
			y = this->getY(this->getContainedY(y));
                        z = z * .002;
                        glRasterPos3f(x, y, z);
                        glutBitmapString(bitmap, (const unsigned char *)text.c_str());
                }

		virtual void setContainedVertex(float x, float y, float z){
			x = this->getContainedX(x);
			y = this->getContainedY(y);
                        z = z * .002;

                        this->setVertex(x, y, z);
                }
		virtual GLuint loadTexture(string texture){
			BmpParser bmp(texture);
			GLuint textureID;
                        glGenTextures(1, &textureID);
                        glBindTexture(GL_TEXTURE_2D, textureID);
                        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
                        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, bmp.bmpStats.width, bmp.bmpStats.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.imageData);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			return textureID;
		}

		virtual void drawTexturedRectangle(string textureFile, float x, float y, float z, float w, float h){
			if(this->texture == 0){
				this->texture = this->loadTexture(textureFile);
			}
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D,this->texture);
			glBegin(GL_QUADS);
				glTexCoord2f( 0, 0 );
                        	this->setVertex(x, y, z);
    				glTexCoord2f( 1, 0 );
                       	 	this->setVertex(x+w, y, z);
    				glTexCoord2f( 1, 1 );
                        	this->setVertex(x+w, y+h, z);
    				glTexCoord2f( 0, 1 );
                        	this->setVertex(x, y+h, z);
                        this->stopDrawing();
			glDisable(GL_TEXTURE_2D);

		}

		virtual void drawRectangle(float x, float y, float z, float w, float h){
                        this->startDrawing();
                        this->setVertex(x, y, z);
                        this->setVertex(x+w, y, z);
                        this->setVertex(x+w, y+h, z);
                        this->setVertex(x, y+h, z);
                        this->stopDrawing();
                }
};

/*
class Object{
	private:
		int objectId = -1;
		string objectName = "";
		float constraintX = 0;
		float constraintY = 0;
		float constraintW = 0;
		float constraintH = 0;
		GLuint texture = 0;
		int displayX = 0;
		int displayY = 0;
		
		void getDisplayResolution(void){
			if(this->displayX == 0 && this->displayY == 0){
                		Display* d = XOpenDisplay(NULL);
                		Screen*  s = DefaultScreenOfDisplay(d);
                		this->displayX = s->width;
                		this->displayY = s->height;
				XCloseDisplay(d);
			}
        	}
	public:
		/*
		 * Setters and Getters.
		 * */
/*		void setConstraints(float x, float y, float w, float h){
			this->constraintX = this->getX(x);
			this->constraintY = this->getY(y);
			this->constraintW = w;
			this->constraintH = h;
		}
		void setObjectId(int id){
			this->objectId = id;
		}
		int getObjectId(void){
			return this->objectId;
		}
		void setObjectName(string name){
			this->objectName = name;
		}
		string getObjectName(void){
			return this->objectName;
		}
		void setConstaintX(float x){
			this->constraintX = x;
		}
		float getConstraintX(){
			return this->constraintX;	
		}
		void setConstraintY(float y){
			this->constraintY = y;
		}
		float getConstraintY(){
			return this->constraintY;
		}
		void setConstraintW(float w){
			this->constraintW = w;
		}
		float getConstraintW(void){
			return this->constraintW;
		}
		void setConstraintH(float h){
			this->constraintH = h;
		}
		float getConstraintH(void){
			return this->constraintH;
		}

		/*
		 * Configuration Functions.
		 * */
/*		float getX(float x){
			this->getDisplayResolution();
                        if(x >=0 && x < this->displayX/2){
                                x = -1 + (x*((9999999/(this->displayX/2))*0.0000001));
                        }else if(x == this->displayY/2){
                                x = 0;
                        }else if(x > this->displayX/2 && x < this->displayX){
                                x = -1 + (x*((9999999/(this->displayX/2))*0.0000001));
                        }else if(x == this->displayX){
                                x = 1;
                        }
                        return x;
                }

                float getMultiple(float s, float p){
                        return s / 1000;
                }
                float getContainedX(float x){
                        x = this->getX(x) + this->constraintX;
                        return x;
                }

                float getY(float y){
			this->getDisplayResolution();
                        if(y >=0 && y < this->displayY/2){
                                y = -1 + (y*((9999999/(this->displayY/2))*0.0000001));
                        }else if(y == this->displayY/2){
                                y = 0;
                        }else if(y > this->displayY/2 && y < this->displayY){
                                y = -1 + (y*((9999999/(this->displayY/2))*0.0000001));
                        }else if(y == this->displayY){
                                y = 1;
                        }
                        return y;
                }

		float getContainedY(float y){
			y = this->getY(y) + this->constraintY;
			return y;
		}
		/*
		 * Virtual Drawing Functions 
		 * */
/*		virtual void drawContainedTextArray(float x, float y, float z, float w, float h, string text, void* bitmap, float spacing, int maxXOverflow){
                        if(text.empty())
                                return;

                        bool overFlowed = false;
                        size_t textArraySize = text.length();
                        if(textArraySize > maxXOverflow){
                                textArraySize = maxXOverflow + 3;
                                overFlowed = true;
                        }

                        string *textArray = new string[textArraySize];

                        for(int i=0; i<textArraySize; i++){
                                if(i < maxXOverflow)
                                        textArray[i] = text[i];
                                else{
                                        textArray[i] = ".";
                                }
                        }

                        for(int i=0; i<textArraySize; i++){
                                if(i != 0)
                                        x += spacing;
                                this->drawContainedText(x, y, z, textArray[i], bitmap);
                        }

                }

		virtual void startDrawing(){
                        glBegin(GL_POLYGON);
                }

                virtual void stopDrawing(){
                        glEnd();
                        glFlush();
                }

		virtual void setColor(float r, float g, float b){
                        glColor3f(r, g, b);
                }

                virtual void setVertex(float x, float y, float z){
			x = this->getX(x);
			y = this->getY(y);
                        z = z * .002;
                        glVertex3f(x, y, z);
                }

		virtual void drawText(float x, float y, float z, string text, void* bitmap){
			x = this->getX(x);
			y = this->getY(y);
                        z = z * .002;

			glRasterPos3f(x, y, z);
			glutBitmapString(bitmap, (const unsigned char *)text.c_str());
		}

		virtual void drawContainedText(float x, float y, float z, string text, void* bitmap){
			x = this->getX(this->getContainedX(x));
			y = this->getY(this->getContainedY(y));
                        z = z * .002;
                        glRasterPos3f(x, y, z);
                        glutBitmapString(bitmap, (const unsigned char *)text.c_str());
                }

		virtual void setContainedVertex(float x, float y, float z){
			x = this->getContainedX(x);
			y = this->getContainedY(y);
                        z = z * .002;

                        this->setVertex(x, y, z);
                }
		virtual GLuint loadTexture(string texture){
			BmpParser bmp(texture);
			GLuint textureID;
                        glGenTextures(1, &textureID);
                        glBindTexture(GL_TEXTURE_2D, textureID);
                        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
                        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, bmp.bmpStats.width, bmp.bmpStats.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.imageData);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
                        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			return textureID;
		}

		virtual void drawTexturedRectangle(string textureFile, float x, float y, float z, float w, float h){
			if(this->texture == 0){
				this->texture = this->loadTexture(textureFile);
			}
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D,this->texture);
			glBegin(GL_QUADS);
				glTexCoord2f( 0, 0 );
                        	this->setVertex(x, y, z);
    				glTexCoord2f( 1, 0 );
                       	 	this->setVertex(x+w, y, z);
    				glTexCoord2f( 1, 1 );
                        	this->setVertex(x+w, y+h, z);
    				glTexCoord2f( 0, 1 );
                        	this->setVertex(x, y+h, z);
                        this->stopDrawing();
			glDisable(GL_TEXTURE_2D);

		}

		virtual void drawRectangle(float x, float y, float z, float w, float h){
                        this->startDrawing();
                        this->setVertex(x, y, z);
                        this->setVertex(x+w, y, z);
                        this->setVertex(x+w, y+h, z);
                        this->setVertex(x, y+h, z);
                        this->stopDrawing();
                }
};*/
