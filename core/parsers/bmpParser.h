struct BmpStats{
	u_int32_t headerSize;
	char *header;
	int32_t width;
	int32_t height;
	u_int16_t colorPanelCount;
	u_int16_t bitsPerPixel;
	u_int32_t compressionMethod;
	u_int32_t rawSize;
	u_int32_t horizontalResolution;
	u_int32_t verticalResolution;
	u_int32_t colorPaletteCount;
	u_int32_t importantColorCount;
	u_int32_t dataStartingPosition;
	
};
class BmpParser{
	private:
		/* Location Constants */
		const int8_t HEADER_IMAGE_DATA = 0x0a;
		const int8_t DIB_HEADER_SIZE = 0x0e;
		const int8_t DIB_WIDTH = 0x12;
		const int8_t DIB_HEIGHT = 0x16;
		const int8_t DIB_COLOR_PANEL_COUNT = 0x1a;
		const int8_t DIB_BITS_PER_PIXEL = 0x1c;
		const int8_t DIB_COMPRESSION_METHOD = 0x1e;
		const int8_t DIB_RAW_IMG_SIZE = 0x22;
		const int8_t DIB_HORIZONTAL_RESOLUTION = 0x26;
		const int8_t DIB_VERTICAL_RESOLUTION = 0x2a;
		const int8_t DIB_COLOR_PALETTE_COUNT = 0x2e;
		const int8_t DIB_IMPORTANT_COLOR_COUNT = 0x32;

		/* Compression Method Types */
		const int8_t COMPRESS_RGB = 0;
		const int8_t COMPRESS_RLE8 = 1;
		const int8_t COMPRESS_RLE4 = 2;
		const int8_t COMPRESS_BITFIELDS = 3;
		const int8_t COMPRESS_JPEG = 4;
		const int8_t COMPRESS_PNG = 5;
		const int8_t COMPRESS_ALPHABITFIELDS = 6;
		const int8_t COMPRESS_CMYK = 11;
		const int8_t COMPRESS_CMYKRLE8 = 12;
		const int8_t COMPRESS_CMYKREL4 = 13;

		/* Functional Variables */
		size_t fileSize = 0;
		char *fileData = NULL;

		bool isBmpFile(void){
			if(fileSize <= 2 || fileData == NULL){
				return false;
			}

			if(fileData[0] == 'B' && fileData[1] == 'M'){
				return true;
			}

			return false;
		}

		void getStats(void){
			this->bmpStats.headerSize = *(u_int32_t*) &(this->fileData[this->DIB_HEADER_SIZE]);
			//char *header;
			this->bmpStats.width = *(int32_t*) &(this->fileData[this->DIB_WIDTH]);
			this->bmpStats.height = *(int32_t*) &(this->fileData[this->DIB_HEIGHT]);
			this->bmpStats.colorPanelCount = *(u_int32_t*) &(this->fileData[this->DIB_COLOR_PANEL_COUNT]);
			this->bmpStats.bitsPerPixel = *(u_int32_t*) &(this->fileData[this->DIB_BITS_PER_PIXEL]);
			this->bmpStats.compressionMethod = *(u_int32_t*) &(this->fileData[DIB_COMPRESSION_METHOD]);
			this->bmpStats.rawSize = *(u_int32_t*) &(this->fileData[this->DIB_RAW_IMG_SIZE]);
			this->bmpStats.horizontalResolution = *(u_int32_t*) &(this->fileData[this->DIB_HORIZONTAL_RESOLUTION]);
			this->bmpStats.verticalResolution = *(u_int32_t*) &(this->fileData[this->DIB_VERTICAL_RESOLUTION]);
			this->bmpStats.colorPaletteCount = *(u_int32_t*) &(this->fileData[this->DIB_COLOR_PALETTE_COUNT]);
			this->bmpStats.importantColorCount = *(u_int32_t*) &(this->fileData[this->DIB_IMPORTANT_COLOR_COUNT]);

			this->bmpStats.dataStartingPosition = *(u_int32_t*) &(this->fileData[this->HEADER_IMAGE_DATA]);

			if(this->bmpStats.rawSize == 0){
				this->bmpStats.rawSize = this->bmpStats.width * this->bmpStats.height * 3;
			}
			if(this->bmpStats.dataStartingPosition == 0){
				this->bmpStats.dataStartingPosition = 54;
			}
			
			this->imageSize = this->bmpStats.rawSize;
			this->imageData = new char[this->imageSize];
			for(int i=0; i<this->bmpStats.rawSize; i++){
				if((i+this->bmpStats.dataStartingPosition) >= this->fileSize){
					fprintf(stderr, "[E] BmpParser private getStats: buffer overflow\n");
					return;
				}
				imageData[i] = fileData[i+bmpStats.dataStartingPosition];
			}
		}
	public:
		size_t imageSize = 0;
		char* imageData = NULL;
		struct BmpStats bmpStats;

		void freeImage(void){
			delete[] this->imageData;
		}

		BmpParser(string fileName){
			struct stat st;
			stat(fileName.c_str(), &st);
			this->fileSize = st.st_size;
			int fd = open(fileName.c_str(), O_RDONLY);
			if(!fd){
				fprintf(stderr, "[E] Failed to open .bmp file.\n");
				return;
			}

			this->fileData = new char[this->fileSize];
			if(read(fd, this->fileData, this->fileSize) != this->fileSize){
				fprintf(stderr, "[E] Failed to read .bmp file\n");
				close(fd);
				delete[] this->fileData;
				return;
			}
			close(fd);
			
			if(!this->isBmpFile()){
				fprintf(stderr, "[E] File isn't a bmp file.\n");
				delete[] this->fileData;
				return;
			}
			
			this->getStats();

			if(this->bmpStats.compressionMethod != this->COMPRESS_RGB && this->bmpStats.compressionMethod != this->COMPRESS_BITFIELDS){
				fprintf(stderr, "[W] Compression method %d is not currently supported. Data may require further processing.\n", this->bmpStats.compressionMethod);
			}

			delete[] fileData;
		}
};
