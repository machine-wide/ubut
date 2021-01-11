# ubut&trade;
ubench + utests = ubut

<font size="1">(it also creates unique and somewhat ridiculously sounding name)</font>

Inspired by [Sheredom](https://twitter.com/sheredom) [UTEST](https://github.com/sheredom/utest.h) and [UBENCH](https://github.com/sheredom/ubench.h)
Please respect the copyright and licsensing from there.

## Why?

To have a single source for common stuf.  Yes, I know that complicates dependancy and delivers SPOF's (Single Point Of Fauilure). But it is also far easier to manage and if things are done properly SPOF become SPOX aka Single Point Of eXcelence.

## How?

- depends on [dbj--simplelog](https://github.com/dbj-systems/dbj--simplelog)
  - that is soft dependancy
    - look into [ubut_print.h](ubut_print.h)
    - I have [windows syslog lib](https://github.com/dbj-data/dbjsysloglib), I plan to use in there too, as an option.
- Windows only, but not patologicaly
  - mainly because of the [cmd.exe console colouring](win_lib.h) solution that imposes Windows 10.0.14393  as a minimal version requirement.
  - that part can be made not required if OS is not Windows
- "fixtures" removed
