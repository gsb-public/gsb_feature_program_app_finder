(function ($) {

  Drupal.behaviors.gsb_feature_program_app_finder = {

    attach: function (context, settings) {

      // Initially, hide the date select
      $('div.form-item-programs-instance-date-select').hide();

      // Handle date (program instance) selection
      $('select.programs-instance-date-select').on('change', function() {
        Drupal.gsb_feature_program_app_finder.instanceSelected();
      });

      // Handle program selection
      $('select.program-find-app-select').on('change', function() {
        Drupal.gsb_feature_program_app_finder.programSelected();
      });

      // Handle click for 'Select a Different Program'
      $('a.programs-different-link').on('click', function(event) {
        event.preventDefault();
        event.stopPropagation();
        // reset the detail values

        $('.programs-text').text('');
        $('select.program-find-app-select').multipleSelect('setSelects', []);
        $('.ms-parent.program-find-app-select').show();
        $('.programs-instructions').show();
        $('div.programs-different').hide();
        $('.programs-instance-title').hide();
        $('.programs-instance-deadline').hide();
        $('.programs-instance-application-url-wrapper').hide();
        $('.programs-instance-sample-app-form-url-wrapper').hide();
        $('div.form-item-programs-instance-date-select').hide();
        $('.programs-disclaim').show();
      });

      // Initialize the (multiple-select based) program selector
      $('select.program-find-app-select').each(function () {
        var $select = $(this);
        $select.multipleSelect({
          single: true,
          placeholder : 'Select a Program'
        });
        $(".program-find-app-select input[type='radio']").hide();
        $(".ms-drop").prepend('<div class="header-text">Scroll Down to View All Programs</div>');
        if (settings.gsb_feature_program_app_finder.program != '') {
          var p_nid = settings.gsb_feature_program_app_finder.program;
          $select.val(p_nid);
          Drupal.gsb_feature_program_app_finder.programSelected();
        }
      });

      Drupal.gsb_feature_program_app_finder.programSelected();
      Drupal.gsb_feature_program_app_finder.instanceSelected();
    }
  };

  Drupal.gsb_feature_program_app_finder = Drupal.gsb_feature_program_app_finder || {};

  /**
   * instanceSelected
   *   - Updates the details for the selected program instance
   */
  Drupal.gsb_feature_program_app_finder.instanceSelected = function() {
    var selected_program_nid = $('select.program-find-app-select option:selected').val();
    var selected_instance_nid = $('select.programs-instance-date-select option:selected').val();
    if (selected_instance_nid == '1') {
      if (selected_program_nid == localStorage.getItem('selected-program-id')) {
        selected_instance_nid = localStorage.getItem('selected-instance-id');
      }
    }
    // store the program and program instance ids in localStorage in case the user
    // returns to this page using the browser back button
    localStorage.setItem('selected-program-id', selected_program_nid);
    localStorage.setItem('selected-instance-id', selected_instance_nid);
    $('select.programs-instance-date-select > option').each(function() {
      $(this).removeAttr('selected');
    });
    $('select.programs-instance-date-select option[value="' + selected_instance_nid + '"]').attr('selected', 'selected');
    $('.program-instance-list li span').each(function() {
      var instance_nid = $(this).attr('data-program-instance-nid');
      // Get the detail info for the selected program instance
      if (selected_instance_nid == instance_nid) {
        Drupal.gsb_feature_program_app_finder.updateDetails($(this));
      }
    });
  }

  /**
   * programSelected
   *   - Updates the details for the selected program
   */
  Drupal.gsb_feature_program_app_finder.programSelected = function() {
    var instance_match_count = 0;
    var instance_titles = [];
    if ($('select.program-find-app-select option:selected').length == 0) {
      return;
    }
    var selected_program_nid = $('select.program-find-app-select option:selected').val();
    $('.program-instance-list li span').each(function() {
      var target_id = $(this).attr('data-program-instance-target-id');
      if (selected_program_nid == target_id) {
        instance_match_count++;
        var instance_nid = $(this).attr('data-program-instance-nid');
        var instance_title = $(this).attr('data-program-instance-full-title');
        instance_titles[instance_titles.length] = {
          key: instance_nid,
          text: instance_title
        };
        Drupal.gsb_feature_program_app_finder.updateDetails($(this));
      }
    });
    $('select.programs-instance-date-select').children().remove();
    if (instance_titles.length) {
      $('select.programs-instance-date-select').append($("<option/>", {
        value: '1',
        text: 'Select Program Dates',
        selected: 'selected'
      }));
      var customSelect = $.find('span.customSelectInner');
      if (customSelect != undefined) {
        $(customSelect).text('Select Program Dates');
      }
    }
    for (var index = 0; index < instance_titles.length; index++) {
      $('select.programs-instance-date-select').append($("<option/>", {
        value: instance_titles[index].key,
        text: instance_titles[index].text,
      }));
    }
    $('select.programs-instance-date-select').val('1');
    if (instance_match_count > 1) {
      $('div.form-item-programs-instance-date-select').show();
      $('.programs-instance-title').hide();
      $('.programs-instance-deadline').hide();
      $('.programs-instance-application-url-wrapper').hide();
      $('.programs-instance-sample-app-form-url-wrapper').hide();
    }
    $('.programs-text').text($('select.program-find-app-select option:selected').text());
    $('.ms-parent.program-find-app-select').hide();
    $('.programs-different').show();
    $('.programs-disclaim').hide();
  };

  /**
   * updateDetails
   *   - Sets the detail values displayed in the App Finder
   */
  Drupal.gsb_feature_program_app_finder.updateDetails = function(item) {
    var instance_nid = item.attr('data-program-instance-nid');
    var instance_title = item.attr('data-program-instance-full-title');
    var application_url = item.attr('data-program-instance-application-url');
    var sample_app_form_url = item.attr('data-program-instance-sample-app-form-url');
    var instance_deadline = item.attr('data-program-instance-deadline');
    instance_deadline = replaceAll(instance_deadline, '^', '<br />');
    $('.programs-instructions').hide();
    $('.programs-instance-title').text(instance_title);
    $('.programs-instance-title').show();
    $('.programs-instance-deadline').html(instance_deadline);
    $('.programs-instance-deadline').show();
    if (application_url) {
      $('.programs-instance-application-url').attr('href', application_url);
      $('.programs-instance-application-url-wrapper').show();
    }
    else {
      $('.programs-instance-application-url-wrapper').hide();
    }
    if (sample_app_form_url) {
      $('.programs-instance-sample-app-form-url').attr('href', sample_app_form_url);
      $('.programs-instance-sample-app-form-url-wrapper').show();
    }
    else {
      $('.programs-instance-sample-app-form-url-wrapper').hide();
    }
  }

  /* Define functin to find and replace specified term with replacement string */
  function replaceAll(str, term, replacement) {
    term = term.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    return str.replace(new RegExp(term, 'g'), replacement);
  }

})(jQuery);
