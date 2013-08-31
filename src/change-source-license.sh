function initialize
{
cat > license-text <<EOF
    <SOURCE-LICENSE>
        [ PA(Peter-Anna) OS ] ========================================

          Peter-Anna OS is a real time OS.
          Peter-Anna OS is made by Peter and Anna.
          Peter and Anna lives in South Korea.
          They love each other so much.

          Peter usually develops PAOS,
          Anna usually feeds hungry Peter.
          Peter sometimes makes money,
          Anna sometimes makes cookie.

          Like shared memory region,
          PAOS is where they live in together.
          They belong to PAOS.
          PAOS belongs to them.

                               03/29/2006
                               Peter and Anna ( kangmo.kim@gmail.com )
        [ License Information ] ======================================
         
          Dual License
            1. GPL
            2. PAOS Commercial License
             
          License Selection Algorithm             
            IF You can open your whole source code
              You are under GPL license
            ELSE
              You are under PAOS Commercial License
            FI
                                
        [ Information on Layering Architecture of PAOS ] =============

          Files in lower layer are not allowed to 
          use,include files in upper layer.

          user    ; user program layer      ; main.c
          pthread ; pthread layer           ; pthread.c
          conc    ; concurrency layer       ; conc_mutex.c conc_cond.c
          evt     ; event layer             ; evt.c
          sched   ; scheduler layer         ; sched.c
          task    ; task layer              ; task.c
          base    ; base layer              ; base.c list.c debug.c
          md      ; machine dependent layer ; md.c md_asm.S uart.c
          config  ; configuration layer     ; config.h
                                
    </SOURCE-LICENSE>
EOF

LIC_FILE_NAME=license-text
}

function destroy
{
   rm license-text
}

function get-indicator-line
{
   FILE_NAME=$1
   INDICATOR=$2
   grep -n "$INDICATOR" $FILE_NAME | sed "s/:/ /g" | awk '{print $1}'
}

function get-first-lic-line
{
   FILE_NAME=$1
   get-indicator-line $FILE_NAME "<SOURCE-LICENSE>"
}

function get-last-lic-line
{
   FILE_NAME=$1
   get-indicator-line $FILE_NAME "</SOURCE-LICENSE>"
}

function replace-license
{
   ORG_FILE_NAME=$1
   # First and Last line of the lincense paragraph in the file
   LIC_FIRST_LINE=`get-first-lic-line $ORG_FILE_NAME`
   LIC_LAST_LINE=`get-last-lic-line $ORG_FILE_NAME`

   ORG_LINES=`wc -l $ORG_FILE_NAME | awk '{print$1}'`

   ORG_HEAD_LINES=`expr $LIC_FIRST_LINE - 1`

   ORG_TAIL_LINES=`expr $ORG_LINES - $LIC_LAST_LINE`

   cp $ORG_FILE_NAME $ORG_FILE_NAME.bak
   head -$ORG_HEAD_LINES $ORG_FILE_NAME >  $ORG_FILE_NAME.new
   cat $LIC_FILE_NAME                   >> $ORG_FILE_NAME.new
   tail -$ORG_TAIL_LINES $ORG_FILE_NAME >> $ORG_FILE_NAME.new
   mv $ORG_FILE_NAME.new $ORG_FILE_NAME
}

initialize

replace-license a.txt

destroy
