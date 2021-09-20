

#define MAX_BUFFERING 1024


struct HTTPData {
    bool completed;
    int size;
    char data[MAX_BUFFERING];
    
    explicit HTTPData();
    ~HTTPData();

    void read();
};
