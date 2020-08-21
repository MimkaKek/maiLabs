#ifndef SHA2_H
#define SHA2_H

#define CHUNK_SIZE 64
#define TOTAL_LEN_LEN 8

class TSha256 {
    public:

        TSha256(const void* input, size_t nlen, int nr);
        
        void Сalc(uint8_t hash[32]);
        
    private:

        int calc_chunk(uint8_t chunk[CHUNK_SIZE]);

        inline uint32_t right_rot(uint32_t value, unsigned int count);

        int rounds;
        const uint8_t * p;
        size_t len;
        size_t total_len;
        int single_one_delivered; /* bool */
        int total_len_delivered; /* bool */
};

#endif /* SHA2_H */
