//DistanceSensor.hpp

class DistanceSensor
{
	public:
		DistanceSensor();
		~DistanceSensor();
		char getDistance(char);
	private:
		int addrFL;
		int addrFR;
		int addrRL;
		int addrRR;
		int distanceFL;
		int distanceFR;
		int distanceRL;
		int distanceRR;
		int fd;
        char distanceReturn(char);
};
