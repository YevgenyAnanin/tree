jQuery(document).ready(function($){
    //var token_deps = [
    //    ['dcs',                         'edit-submitted-automation-technologies has dcs'],
    //    ['plc',                         'edit-submitted-automation-technologies has plc'],
    //    ['mes',                         'edit-submitted-automation-technologies has mes'],
    //    ['hmi',                         'edit-submitted-automation-technologies has hmi'],
    //    ['scada',                       'edit-submitted-automation-technologies has scada'],
    //    ['manufacturing-intelligence',  'edit-submitted-automation-technologies has manufacturing_intelligence'],
    //    ['batch-management-software',   'edit-submitted-automation-technologies has batch_management_software'],
    //    ['workflow-software',           'edit-submitted-automation-technologies has workflow_software'],
    //    ['operator-training-software',  'edit-submitted-automation-technologies has operator_training_software'],
    //    ['simulation-software',         'edit-submitted-automation-technologies has simulation_software'],
    //    ['optimization-software',       'edit-submitted-automation-technologies has optimization_software'],
    //]

    //var dependees = [];
    //var tokens = [];
    //$('#edit-submitted-automation-technologies').change(function() {
    //    var has_patt = /([\w-]+)\s+has\s+([^\s;]+)/;
    //    var test_str = token_deps[0][1].replace(has_patt, "$('#$1').val().indexOf('$2') != -1");
    //    alert(eval(test_str));
    //});

    //for (i = 0; i < token_deps.length; i++) {
    //    $('#edit-submitted-automation-technologies').change(function() {
    //        var has_patt = /([\w-]+) has ([^\s]+)/;
    //        var test_str = token_deps[i][1].replace(has_patt, "$('#$1').val().indexOf('$2') != -1");
    //        alert(eval(test_str));
    //    });
    //    //var matches = has_patt.exec(token_deps[i][1]);
    //    //if (matches.length == 3) {
    //    //    tokens[token_deps[i][0]] = eval();
    //    //}
    //}

    function activate(c) {
        $('#edit-submitted-'+c+' .was-required').removeClass('was-required').addClass('required');
        $('#webform-component-'+c).show();
    }

    function deactivate(c) {
        $('#edit-submitted-'+c).removeClass('required').addClass('was-required');
        $('#webform-component-'+c).hide();
    }

    $('#edit-submitted-automation-technologies').change(function() {
        var values = $(this).val();
        if (values && values.indexOf('dcs') != -1) {
            activate('tech-vendor-dcs');
            activate('tech-model-dcs');
        } else {
            deactivate('tech-vendor-dcs');
            deactivate('tech-model-dcs');
        }
    });
});
