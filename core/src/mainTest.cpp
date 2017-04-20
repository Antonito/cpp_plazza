#include "Worker.hpp"

int main(int ac, char **av)
{
    Worker worker;
    Order order;

    if (ac > 1)
    {
        order.addFile(av[1]);
        order.setInfo(Information::PHONE_NUMBER);
        worker.exec(order);
    }
}