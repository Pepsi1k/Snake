class Route
{
  public:
	bool left;
	bool right;
	bool up;
	bool down;

	Route();
	Route& operator=(int value);
};