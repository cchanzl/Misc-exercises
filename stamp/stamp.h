enum MessageStatus {
  INVALID_HEADER=-1,
  WRONG_RECIPIENT=-2,
  INVALID_MESSAGE_DIGEST=-3,
  INVALID_HEADER_DIGEST=-4,
  VALID_EMAIL=1
};

/* pre-supplied helper function */
/* generates the SHA1 hash of input string text into output parameter digest */
void text_to_SHA1_digest(const char *text, char *digest);

// returns number of leadings zeroes. -1 if invalid
int leading_zeros(std::string digest);

// return true if SHA1 digest of the contents can be copied. false if otherwise
bool file_to_SHA1_digest(const std::string filename, char* digest);

// returns false if file cannot be read or counter exceeds 10 million. return true otherwise
bool make_header(const std::string recipient,
		 const std::string filename, char* header);

// check message received 
MessageStatus check_header(std::string email_address,
			   std::string header, std::string filename);
