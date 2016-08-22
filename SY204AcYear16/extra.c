/*	//OPEN FILE WITH CONTENT: check to see if file is empty and then print contents
	int openFunc(char *filename){
		int returnVal = 1;
		//int count = 0;
		FILE *file = fopen(filename, 'r');
		if (fgetc(file)!= EOF){
			//while (sizeof(filename) > count){
				//printf(fgetc(file));
				//count++;
			returnVal = 0;
			//}
		}
		else{
			printf("File has no content");
			exit(12);
		}
	return returnVal;
	}

*/

/*  //char *filename;
        if (argc  > 1){
          while (read > 0){ //read will tell you number of bytes read, 0 for all, -1 for error
  write(1, argv[count], strlen(argv[count])); //print its contents to standard out (1)
  //close(argv[count]);
   count++;
           }
          //ERROR
          //if (read < 0){
            //write(2, );
            //exit(99);
         // }
   }
          if (argc < 1){
  write(2, argv[0], strlen(argv[0]));
  //write(2, 4721);
  //exit(489);
          }

      }
      */
