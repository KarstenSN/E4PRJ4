#include "PcCom.hpp"

PcCom::PcCom(dataTest* dataClassPtr, settingsTest* settingsTestPtr)
{
    int i;
    for(i = 0; i < 4; i++)
        this->controller_[i] = 0;
    for(i = 0; i < 6; i++)
        this->data_[i] = 0;
    this->portnoData_ = 1234;
    this->portnoController_ = 1235;
    this->dataClassPtr_ = dataClassPtr;
    this->settingsClassPtr_ = settingsTestPtr;
    this->UserInput_.forward = 0;
    this->UserInput_.reverse = 0;
    this->UserInput_.stop = 0;
    this->UserInput_.turn = 0;
}

PcCom::~PcCom()
{

}

int PcCom::controllerStream()
{
    // Initialize variables
    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Initialize socket for controllerStream
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       this->error("ERROR opening socket");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->portnoController_);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        this->error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // Infinite wait for connection
    while(1)
    {
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            break;

        while(1)
        {
            /* Acquire controller data from computer
             * controller[0] = Forward      (0 - 255)
             * controller[1] = Reverse      (0 - 255)
             * controller[2] = Turn         (-127 - 128)
             * controller[3] = Break        (0 - 1) */
            n = read(newsockfd,this->controller_,4);

            if (n < 0)
                this->error("ERROR reading from socket");

            this->UserInput_.forward = this->controller_[0];
            this->UserInput_.reverse = this->controller_[1];
            this->UserInput_.turn = this->controller_[2];
            this->UserInput_.stop = this->controller_[3];

            //TODO indsæt variable i data klasse med nye kommando

            // Send last received controller data
            n = write(newsockfd,this->controller_,4);

            if (n < 0)
                break;

        }
        if (n < 0)
            this->error("ERROR writing to socket");

        close(newsockfd);

    }
    close(sockfd);
    return 0;

}

int PcCom::dataStream()
{
    std::cout << "Initializing TCP server for datastream" << std::endl;
    // Initialize variables
    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char distance;

    // Initialize socket for dataStream
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       this->error("ERROR opening socket");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->portnoData_);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        this->error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    std::cout << "Initialization Complete" << std::endl;

    // Infinite wait for connection
    while(1)
    {
        std::cout << "Waiting for Connection" << std::endl;

        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            break;

        std::cout << "Connection aquired" << std::endl;
        while(1)
        {
            /*  Acquire data from computer:
             *  data[0] = Max speed             (km/h)
             *  data[1] = Speed                 (km/h * 10)
             *  data[2] = Distance              (m * 10)
             *  data[3] = Acceleration          (G * 10)
             *  data[4] = AKS-status            (true/false)
             *  data[5] = Steering calibration  (-50 - 50) */
            n = read(newsockfd,this->data_,6);

            // Shut down if "dwnnow" is send by the PC
            if(this->data_[0] == 'd' && this->data_[1] == 'w' && this->data_[2] == 'n' && this->data_[3] == 'n' && this->data_[4] == 'o' && this->data_[5] == 'w')
                break;

            std::cout << "Read new data from PC" << std::endl;
            std::cout << "Cal: " << static_cast<int>(this->data_[5]) << "  AKS: " << static_cast<int>(this->data_[4]) << "  Max: " << static_cast<int>(this->data_[0]) << std::endl;

            if (n < 0)
                this->error("ERROR reading from socket");

            // Update the steering calibration in Settings
            this->settingsClassPtr_->calibrateSteering(this->data_[5]);

            // Update the AKS status in Settings
            if (this->data_[4] > 0)
            {
                this->settingsClassPtr_->setAKS(true);
            }
            else
            {
                this->settingsClassPtr_->setAKS(false);
            }

            // Update the maximum speed in Settings
            this->settingsClassPtr_->setMaxSpeed((int)this->data_[0]);

            // Get latest Velocity and put in data buffer
            this->data_[1] = static_cast<char>(this->dataClassPtr_->getLatestVelocity() * 10);

            // Get latest distances and put closest one in data buffer
            distance = this->dataClassPtr_->getLatestDistance("FR");
            if (this->dataClassPtr_->getLatestDistance("FL") > distance)
                distance = this->dataClassPtr_->getLatestDistance("FL");
            if (this->dataClassPtr_->getLatestDistance("RL") > distance)
                distance = this->dataClassPtr_->getLatestDistance("RL");
            if (this->dataClassPtr_->getLatestDistance("RR") > distance)
                distance = this->dataClassPtr_->getLatestDistance("RR");

            this->data_[2] = static_cast<char>(distance * 10);

            // Get latest Acceleration and put in data buffer
            this->data_[3] = static_cast<char>(this->dataClassPtr_->getLatestAcceleration() * 10);

            std::cout << "Sending data: " << std::endl;
            std::cout << "Vel: " << static_cast<int>(this->data_[1]) << "  Dis: " << static_cast<int>(this->data_[2]) << "  Acc: " << static_cast<int>(this->data_[3]) << std::endl;

            // Send new data
            n = write(newsockfd,this->data_,6);

             if (n < 0)
                 break;


        }
        n = write(newsockfd,this->data_,6);
        if (n < 0)
            this->error("ERROR writing to socket");
        close(newsockfd);

        // Shut down if "dwnnow" is send by the PC
        if(this->data_[0] == 'd' && this->data_[1] == 'w' && this->data_[2] == 'n' && this->data_[3] == 'n' && this->data_[4] == 'o' && this->data_[5] == 'w')
            break;
    }
    std::cout << "Closing the data coneection" << std::endl;
    close(sockfd);
    return 0;

}

void PcCom::error(const char *msg)
{
    perror( msg );
    exit(1);
}
