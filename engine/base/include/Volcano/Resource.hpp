//
//
#ifndef VOLCANO_RESOURCE_HPP
#define VOLCANO_RESOURCE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Buffer.hpp>

VOLCANO_BEGIN

class Resource: public Buffer {
public:
	enum class State {
		Empty = 0,
		Loading,
		Ready,
		Error
	};

public:
	Resource(void);
	~Resource(void) override;

public:
	int flags(void) override;
	bool open(int modes) override;
	State state(void) const;
	bool startLoading(void);
	void cancelLoading(void);
	int loadingProgress(void) const;

protected:
	int64_t writeData(const void *buf, int64_t len) override;

private:
	ByteArray m_data;
	State m_state;
	int m_loadingProgress;
};

VOLCANO_END

#endif // VOLCANO_RESOURCE_HPP
