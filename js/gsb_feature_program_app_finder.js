(function ($) {

  Drupal.behaviors.gsb_feature_program_app_finder = {

    attach: function (context, settings) {

      $('select.program-find-app-select').on('change', function() {
        Drupal.gsb_feature_program_app_finder.programSelected($(this).val());
      });

      $('a.programs-different-link').on('click', function(event) {
        event.preventDefault();
        event.stopPropagation();
        $('.programs-text').text('');
        $('select.program-find-app-select').multipleSelect('setSelects', []);
        $('.ms-parent.program-find-app-select').show();
        $('div.programs-different').hide();
        $('.programs-instance-title').hide();
        $('.programs-instance-deadline').hide();
        $('.programs-instance-application-url-wrapper').hide();
        $('.programs-instance-sample-app-form-url-wrapper').hide();
        $('.programs-disclaim').show();
      });

      $('select.program-find-app-select').each(function () {
        var $select = $(this);
        $select.multipleSelect({
          single: true
        });
        if (settings.gsb_feature_program_app_finder.program != '') {
          var p_nid = settings.gsb_feature_program_app_finder.program;
          $select.val(p_nid);
          Drupal.gsb_feature_program_app_finder.programSelected(p_nid);
        }
      });

    }
  };

  Drupal.gsb_feature_program_app_finder = Drupal.gsb_feature_program_app_finder || {};

  Drupal.gsb_feature_program_app_finder.programSelected = function(p_nid) {
    $('.program-instance-list li span').each(function() {
      var selected_program_nid = $('select.program-find-app-select option:selected').val();
      var target_id = $(this).attr('data-program-instance-target-id');
      if (selected_program_nid == target_id) {
        var instance_title = $(this).attr('data-program-instance-full-title');
        var application_url = $(this).attr('data-program-instance-application-url');
        var sample_app_form_url = $(this).attr('data-program-instance-sample-app-form-url');
        $('.programs-instance-title').text(instance_title);
        $('.programs-instance-title').show();
        var instance_deadline = $(this).attr('data-program-instance-deadline');
        $('.programs-instance-deadline').text(instance_deadline);
        $('.programs-instance-deadline').show();
        if (application_url) {
          $('.programs-instance-application-url').attr('href', application_url);
          $('.programs-instance-application-url-wrapper').show();
        }
        else {
          $('.programs-instance-application-url-wrapper').show();
        }
        if (sample_app_form_url) {
          $('.programs-instance-sample-app-form-url').attr('href', sample_app_form_url);
          $('.programs-instance-sample-app-form-url-wrapper').show();
        }
        else {
          $('.programs-instance-sample-app-form-url-wrapper').hide();
        }
      }
    });
    $('.programs-text').text($('select.program-find-app-select option:selected').text());
    $('.ms-parent.program-find-app-select').hide();
    $('.programs-different').show();
    $('.programs-disclaim').hide();
  };

})(jQuery);