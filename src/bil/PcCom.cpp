#include <PcCom.hpp>

PcCom::PcCom(Data* dataClassPtr, Settings* settingsClassPtr, Log* logClassPtr)
{
    int i;
    for(i = 0; i < 4; i++)
        this->controller_[i] = 0;
    for(i = 0; i < 6; i++)
        this->data_[i] = 0;
    this->portnoData_ = 1234;
    this->portnoController_ = 1235;
    this->dataClassPtr_ = dataClassPtr;
    this->settingsClassPtr_ = settingsClassPtr;
    this->logClassPtr_ = logClassPtr;
    this->UserInput_.forward = 0;
    this->UserInput_.reverse = 0;
    this->UserInput_.stop = 0;
    this->UserInput_.turn = 0;
    
    std::thread dataStreamTh(&PcCom::dataStream , this);
    std::thread controllerStreamTh(&PcCom::controllerStream , this);
    
    this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__,"PcCom initialized");
}

PcCom::~PcCom()
{

}

void PcCom::controllerStream()
{
    this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__,"Initializing TCP connection for user input stream");
  
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

        this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__, "Controller TCP-Connection aquired");
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

            // Add controller data to the
            this->UserInput_.forward = this->controller_[0];
            this->UserInput_.reverse = this->controller_[1];
            this->UserInput_.turn = this->controller_[2];
            this->UserInput_.stop = this->controller_[3];

            // Variables is send to data class through the struct
            this->dataClassPtr_->writeUserInput(&this->UserInput_);

            // Send last received controller data
            n = write(newsockfd,this->controller_,4);

            if (n < 0)
            {
                this->error("ERROR writing to socket");
                break;
            }
        }
        close(newsockfd);
    }
    close(sockfd);
    return;
}

void PcCom::dataStream()
{
    this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__, "Initializing TCP connection for data stream");
    
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

    // Infinite wait for connection
    while(1)
    {
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            break;

        this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__, "Data TCP-Connection aquired");
        while(1)
        {
            /*  Acquire data from computer:
             *  data[0] = Max speed             (km/h)
             *  data[1] = Speed                 (km/h * 10)
             *  data[2] = Distance              (m * 10)
             *  data[3] = Acceleration          (G * 10)
             *  data[4] = AKS-status            (0 - 1)
             *  data[5] = Steering calibration  (-50 - 50) */
            n = read(newsockfd,this->data_,6);

            // Shut down if "dwnnow" is send by the PC
            if(this->data_[0] == 'd' && this->data_[1] == 'w' && this->data_[2] == 'n' && this->data_[3] == 'n' && this->data_[4] == 'o' && this->data_[5] == 'w')
                break;

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
            this->settingsClassPtr_->setMaxSpeed(static_cast<int>(this->data_[0]));

            // Get latest Velocity and put in data buffer
            this->data_[1] = static_cast<char>(this->dataClassPtr_->getLatestVelocity());

            // Get latest distances and put lowest value in data buffer
            distance = this->dataClassPtr_->getLatestDistance("FR");
            if (this->dataClassPtr_->getLatestDistance("FL") > distance)
                distance = this->dataClassPtr_->getLatestDistance("FL");
            if (this->dataClassPtr_->getLatestDistance("RL") > distance)
                distance = this->dataClassPtr_->getLatestDistance("RL");
            if (this->dataClassPtr_->getLatestDistance("RR") > distance)
                distance = this->dataClassPtr_->getLatestDistance("RR");

            this->data_[2] = static_cast<char>(distance);

            // Get latest Acceleration and put in data buffer
            this->data_[3] = static_cast<char>(this->dataClassPtr_->getLatestAcceleration());

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
    this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__,"Closing the data coneection");
    close(sockfd);
    return;
}

void PcCom::error(std::string msg)
{
    this->logClassPtr_->writeError(__PRETTY_FUNCTION__,msg);
    exit(1);
}
