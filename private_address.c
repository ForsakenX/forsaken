/* Public domain code by The Lion. */

/* Assuming little-endian byte order (all intel-compatible...) */

/* Converts a uint32 between network (big-endian) byte order or backwards. */
#define NBO(x) ( \
	(((x) & 0x000000ff) << 24) | \
	(((x) & 0x0000ff00) << 8)  | \
	(((x) & 0x00ff0000) >> 8)  | \
	(((x) & 0xff000000) >> 24)   \
)

/* Convert prefix length to mask, e.g. 24 -> 0xFFFFFF00, 8 -> 0xFF000000 */
#define MASK(x) (~((1<<(32-(x)))-1))

typedef int uint32;

typedef struct {
	uint32 address;
	uint32 mask;
} netblock_t;

#define NUM_PRIVATE_BLOCKS 6

static const netblock_t private_blocks[] = {
	 { NBO(0x00000000), NBO(MASK(8))  }
	,{ NBO(0x0a000000), NBO(MASK(8))  } /* 10.x.x.x (private) */
	,{ NBO(0x7f000000), NBO(MASK(8))  } /* 127.x.x.x (loopback) */
	,{ NBO(0xa9fe0000), NBO(MASK(16)) } /* 169.254.x.x (zeroconf) */
	,{ NBO(0xac100000), NBO(MASK(12)) } /* 172.16.x.x - 172.31.x.x (private) */
	,{ NBO(0xc0a80000), NBO(MASK(16)) } /* 192.168.x.x (private) */
};

int private_address( uint32 host ) {
	int i;

	for( i = 0; i < NUM_PRIVATE_BLOCKS; i++ ) {
		if( ( host & private_blocks[i].mask ) == private_blocks[i].address ) {
			return 1;
		}
	}

	return 0;
}
