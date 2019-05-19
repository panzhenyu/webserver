#include <stdio.h>

void doPost(struct HttpRequest *req, struct HttpResponse *res)
{
    printf("doPost\n");
    // HttpResponse_redirect(res, "http://localhost:9000/a.png");
}