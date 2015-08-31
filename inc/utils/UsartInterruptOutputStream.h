#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of an output stream for the serial USART.
   * Writing to this stream causes data to be transmitted to the USART.
   */
  template<class T>
  class UsartInterruptOutputStream : public OutputStream {

    protected:
	  T& _usart;

    public:
      UsartInterruptOutputStream(T& usart): _usart(usart) {}
      virtual ~UsartInterruptOutputStream() {}

      // overrides from OutputStream

      virtual bool write(uint8_t c) override {
    	  _usart.putch(c);
    	  return true;
      }
      virtual bool write(const void *buffer,uint32_t size) override{

    	  const uint8_t *ptr;

    	  ptr=static_cast<const uint8_t *>(buffer);

    	  while(size--)
    		  _usart.putch(*ptr++);

    	  return true;

      }

      /**
       * Always true.
       * @return always true
       */

      virtual bool flush() override {
        return true;
      }

      /**
       * Always true.
       * @return always true
       */

      virtual bool close() override {
        return true;
      }
  };
}
