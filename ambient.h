

struct ambient_data {
    double temp;
    double rh;
    double pressure;
    int co2;
};

int ambient_send(const char* channel_id, const char* write_key, struct ambient_data* data);
