** END HEADER - don't remove this line
//
// created on: 2019-03-13
//
parameter bModal, pa2
local f
f = new Form1()
if (bModal)
    f.mdi = .f.    && ensure mdi
    f.ReadModal()
else
    f.Open()
endif

CLASS Form1 OF Form

    PRIVATE
    LOCAL

ENDCLASS
