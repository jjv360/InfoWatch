
var crc32table;

// Modified, from http://stackoverflow.com/a/18639999/1008736
module.exports = function(input) {
	
	// Make sure hash table is created
	if (!crc32table) {
		
		// Create hash table
		var c;
		crc32table = [];
		for(var n =0; n < 256; n++){
			c = n;
			for(var k =0; k < 8; k++){
				c = ((c&1) ? (0xEDB88320 ^ (c >>> 1)) : (c >>> 1));
			}
			crc32table[n] = c;
		}
		
	}
	
	// Calculate CRC32 from string
	var crc = 0 ^ (-1);
	for (var i = 0; i < input.length; i++ )
		crc = (crc >>> 8) ^ crc32table[(crc ^ input.charCodeAt(i)) & 0xFF];

	return (crc ^ (-1)) >>> 0;
	
};