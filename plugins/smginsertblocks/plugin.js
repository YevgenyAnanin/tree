/**
 * @file Plugin description
 */
( function() {
    
  CKEDITOR.plugins.add('smginsertblocks', 
  {
  
    init: function(editor) {
      
      var addCssObj = CKEDITOR;
      
      // Add the styles that renders our fake objects.
      addCssObj.addCss(
        'img.cke_drupal_insert_blocks' +
        '{' +
        'background-position: center center;' +
        'background-repeat: no-repeat;' +
        'clear: both;' +
        'display: block;' +
        'padding: 20px;' +
        'float: none;' +
        'width: 100%;' +
        'border-top: red 1px dotted;' +
        'border-bottom: red 1px dotted;' +
        'height: 5px;' +
        '}'
        );

      // Adding button
      editor.ui.addButton('SMGInsertBlock', {
          label: Drupal.t('Insert Blocks'),
          icon: this.path + 'images/smginsertblocks.png',
          command: 'insertblocksDialog'
      });
           
     editor.addCommand('insertblocksDialog', new CKEDITOR.dialogCommand('insertblocksDialog'));
     
     CKEDITOR.dialog.add('insertblocksDialog', this.path + 'dialogs/smginsertblocks.js');

    },
    afterInit : function( editor )
    {
      // Adds the comment processing rules to the data filter, so comments
      // are replaced by fake elements.
      editor.dataProcessor.dataFilter.addRules(
      {
        comment : function( value )
        {
          if ( !CKEDITOR.htmlParser.comment.prototype.getAttribute ) {
            CKEDITOR.htmlParser.comment.prototype.getAttribute = function() {
              return '';
            };
            CKEDITOR.htmlParser.comment.prototype.attributes = {
              align : ''
            };
          }

          if (value.indexOf('smgInsertedBlock') > -1 )
            return editor.createFakeParserElement( new CKEDITOR.htmlParser.comment( value ), 'cke_drupal_insert_blocks', 'hr' );

          return value;
        }
      });
    }
    
  });
  
})();