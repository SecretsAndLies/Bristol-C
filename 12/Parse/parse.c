

int main( int argc, char *argv[] )  
{

    return 0;
}

void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file");
   }
   return fp;
}

// void* ncalloc(int n, size_t size)
// {
//    void* v = calloc(n, size);
//    if(v==NULL){
//       on_error("Cannot calloc() space");
//    }
//    return v;
// }

// void* nrecalloc(void* p, int oldbytes, int newbytes)
// {
//    void* n = calloc(newbytes, 1);
//    if(n==NULL){
//       on_error("Cannot calloc() space");
//    }
//    memcpy(n, p, oldbytes);
//    free(p);
//    return n;
// }

// void* nremalloc(void* p, int bytes)
// {
//    void* n = realloc(p, bytes);
//    if(n==NULL){
//       on_error("Cannot malloc() space");
//    }
//    return n;
// }
