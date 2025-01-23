struct statu{
    int status;

    statu(int status){
        this->status = status;
    }
    void updateStatus(int add) {
        status = status+add;
    }
}