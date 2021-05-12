//
//
#ifndef VOLCANO_IO_HPP
#define VOLCANO_IO_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class IO {
	VOLCANO_DISABLE_COPY_AND_MOVE(IO)

public:
	enum {
		FlagReadable = 0x1,
		FlagWritable = 0x2,
		FlagSeekable = 0x4,
		FlagMappable = 0x8
	};

	enum {
		ModeRead = 0x1,
		ModeWrite = 0x2,
		ModeAppend = 0x4,
		ModeTruncate = 0x8,
		ModeNewOnly = 0x10
	};

	enum class SeekMode {
		Set = 0,
		Cur,
		End
	};

	class Mapper {
	public:
		Mapper(Mapper &&that);
		virtual ~Mapper(void);

	public:
		int64_t size(void) const;
		void *data(void);
		bool isValid(void) const;
		IO &io(void);

	public:
		Mapper &operator=(Mapper &&that);

	private:
		friend class IO;
		Mapper(IO &io, void *data, int64_t size);

	private:
		IO &m_io;
		int64_t m_size;
		void *m_data;
	};

public:
	IO(void);
	virtual ~IO(void);

public:
	virtual int flags(void) = 0;
	virtual int64_t size(void) = 0;
	virtual bool open(int modes);
	virtual void close(void);
	bool isOpen(void) const;
	int modes(void) const;
	int64_t tell(void);
	int64_t seek(int64_t offset, SeekMode seekMode = SeekMode::Set);
	int64_t read(void *buf, int64_t len);
	int64_t write(const void *buf, int64_t len);
	Mapper map(int64_t offset, int64_t len);

protected:
	virtual int64_t pos(void) = 0;
	virtual bool setPos(int64_t pos) = 0;
	virtual int64_t readData(void *buf, int64_t len) = 0;
	virtual int64_t writeData(const void *buf, int64_t len) = 0;
	virtual void *createMap(int64_t offset, int64_t len) = 0;
	virtual void releaseMap(void *p, int64_t len) = 0;

private:
	int m_modes;

	friend class Mapper;
};

VOLCANO_END

#endif // VOLCANO_IO_HPP
