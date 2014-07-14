/**
 * @file
 * SNG Insert Block Dialog
 */

(function() {
  
  var smgInsertBlockDialog = function(editor) {
    return {
      title: 'SMG Insert Blocks',
      minWidth: 400,
      minHeight: 200,
      contents: 
      [
        {
          id: 'tab-basic',
          label: 'Basic Settings',
          elements: 
          [
            {
              type: 'select',
              id: 'block_name',
              className : 'block_name',
              label: 'Choose Which Block You Want To Insert',
              items: [],
              onLoad : function(){
                 /*
                 return json list of blocks that you want to allow
                 to be inserted via block-insert-list url.
                
                Example:
                $block_list = array(
                  array(
                    'name' => '', 
                    'value' => '',
                  ),
                  array(
                    'name' => 'Smg Related',
                    'value' => 'smg_global,smg_related',
                  ),
                  array(
                    'name' => 'Ad 1',
                    'value' => 'block, 1',
                  ),
                );
                return $block_list;
                */
                (function($){
                  $.ajax({
                    url: "/block-insert-list",
                    dataType: 'json',
                    data: "",
                    success: function(data){
                      $.each(data, function(key, val){
                          $(".block_name").append(
                              "<option value='"+val.value+"'>"+val.name+"</option>"
                          );
                      });
                    }
                  });
                })(jQuery);
              }
            } 
          ]
        }
      ],
      onOk : function()
			{
			  var dialog = this;
			  insertComment('%%smgInsertedBlock:' + dialog.getValueOf( 'tab-basic','block_name') + '%%');
			}
    };
    
    // This function effectively inserts the comment into the editor.
    function insertComment( text )
    {
      // Create the fake element that will be inserted into the document.
      // The trick is declaring it as an <hr>, so it will behave like a
      // block element (and in effect it behaves much like an <hr>).
      if ( !CKEDITOR.dom.comment.prototype.getAttribute ) {
        CKEDITOR.dom.comment.prototype.getAttribute = function() {
          return '';
        };
        CKEDITOR.dom.comment.prototype.attributes = {
          align : ''
        };
      }
      var fakeElement = editor.createFakeElement( new CKEDITOR.dom.comment( text ), 'cke_drupal_insert_blocks', 'hr' );

      // This is the trick part. We can't use editor.insertElement()
      // because we need to put the comment directly at <body> level.
      // We need to do range manipulation for that.

      // Get a DOM range from the current selection.
      var range = editor.getSelection().getRanges()[0],
      elementsPath = new CKEDITOR.dom.elementPath( range.getCommonAncestor( true ) ),
      element = ( elementsPath.block && elementsPath.block.getParent() ) || elementsPath.blockLimit,
      hasMoved;

      // If we're not in <body> go moving the position to after the
      // elements until reaching it. This may happen when inside tables,
      // lists, blockquotes, etc.
      while ( element && element.getName() != 'body' )
      {
        range.moveToPosition( element, CKEDITOR.POSITION_AFTER_END );
        hasMoved = 1;
        element = element.getParent();
      }

      // Split the current block.
      if ( !hasMoved )
        range.splitBlock( 'p' );

      // Insert the fake element into the document.
      range.insertNode( fakeElement );

      // Now, we move the selection to the best possible place following
      // our fake element.
      var next = fakeElement;
      while ( ( next = next.getNext() ) && !range.moveToElementEditStart( next ) )
      {}

      range.select();
    }
  };
  
  CKEDITOR.dialog.add('insertblocksDialog', function(editor) {
    return smgInsertBlockDialog(editor);
  });
    
})();