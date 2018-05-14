<?php
/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup templates
 */

// GENERAL
global $base_url;
$missing = '<span class="label label-danger">missing</span>';

// CARRIER
$carrier_percent = 10;
$mc = render($content['field_mctext']); if(empty($mc)) { $mc = $missing; $carrier_percent--; } 
$dot = render($content['field_dot']); if(empty($dot)) { $dot = $missing; $carrier_percent--; }
$taxid = render($content['field_taxid']); if(empty($taxid)) { $taxid = $missing; $carrier_percent--; }
$ifta = render($content['field_ifta_number']); if(empty($ifta)) { $ifta = $missing; $carrier_percent--; }
$rate = render($content['field_rate']); if(empty($rate)) { $rate = $missing; $carrier_percent--; }
$fac_acct = render($content['field_uaccount']); if(empty($fac_acct)) { $fac_acct = $missing; $carrier_percent--; }
$fac_pass = render($content['field_password']); if(empty($fac_pass)) { $fac_pass = $missing; $carrier_percent--; }
$phone = render($content['field_phone']); if(empty($phone)) { $phone = $missing; $carrier_percent--; }
$ext = render($content['field_ext1']);
$email = render($content['field_email']); if(empty($email)) { $email = $missing; $missing; $carrier_percent--; }
$address = render($content['field_address']); if(empty($address)) { $address = $missing; $carrier_percent--; }
$comments = render($content['body']); if(empty($comments)) $comments = $missing;
$audited = render($content['field_audited']); if(empty($audited)) $audited = $missing; 
$notify = render($content['field_notify']); if(empty($notify)) $notify = $missing; 
$operate = render($content['field_operate']); if(empty($operate)) $operate = $missing; 
$edit = '<a class="btn btn-primary btn-sm" role="button" href="'.$base_url.'/node/'.$node->nid.'/edit">edit</a>';
$carrier_percent = round(($carrier_percent * 100) / 10, 1);
$carrier_color = 'progress-bar-danger';
if ($carrier_percent > 30) $carrier_color = 'progress-bar-warning';
if ($carrier_percent > 79) $carrier_color = 'progress-bar-success';

// Permits
$irp = render($content['field_irp_number']); if(empty($irp)) { $irp = $missing; }
$nj = render($content['field_nj_certification']); if(empty($nj)) { $nj = $missing; }
$nm = render($content['field_nm_permit']); if(empty($nm)) { $nm = $missing; } 
$ny = render($content['field_ny_permit']); if(empty($ny)) { $ny = $missing; }
$kyu = render($content['field_kyu_permit']); if(empty($kyu)) { $kyu = $missing; }
$ucr = render($content['field_ucr_permit']); if(empty($ucr)) { $ucr = $missing; }

// Insurance Information
$ia_nid = isset($node->field_insurance['und'][0]['target_id']) ? $node->field_insurance['und'][0]['target_id'] : '';
$ins_com = $missing; $ins_agent = $missing; $ins_email = $missing; $ins_phone = $missing; 
$ins_ext = $missing; $ins_add = $missing;   $ins_comm = $missing;  $ins_edit = $missing; 
if(!empty($ia_nid)){
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['#node']->field_cname['und'][0]['value']))
    $ins_agent = $content['field_insurance'][0]['node'][$ia_nid]['#node']->field_cname['und'][0]['value'] . ' ';
  $ins_agent .= $content['field_insurance'][0]['node'][$ia_nid]['#node']->title; 
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['field_insurance_company']['#items'][0]['entity']->title))
    $ins_com = $content['field_insurance'][0]['node'][$ia_nid]['field_insurance_company']['#items'][0]['entity']->title;
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['field_email'][0]['#markup']))
    $ins_email = $content['field_insurance'][0]['node'][$ia_nid]['field_email'][0]['#markup'];
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['field_phone'][0]['#markup']))
    $ins_phone = $content['field_insurance'][0]['node'][$ia_nid]['field_phone'][0]['#markup'];
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['field_ext1'][0]['#markup']))
    $ins_ext = $content['field_insurance'][0]['node'][$ia_nid]['field_ext1'][0]['#markup'];
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['field_address'][0]['#address'])){
    $insadd = $content['field_insurance'][0]['node'][$ia_nid]['field_address'][0]['#address'];
    $istreet = isset($insadd['thoroughfare']) ? $insadd['thoroughfare'] : '';
    $icity = isset($insadd['locality']) ? $insadd['locality'] : '';
    $istate = isset($insadd['administrative_area']) ? $insadd['administrative_area'] : '';
    $izipc = isset($insadd['postal_code']) ? $insadd['postal_code'] : '';
    $ins_add = $istreet;
    $ins_add .= !empty($ins_add) && !empty($icity)  ? ', ' . $icity : $icity;  
    $ins_add .= !empty($ins_add) && !empty($istate) ? ', ' . $istate : $istate;  
    $ins_add .= !empty($izipc) ? ' ' . $izipc : '';  
    $ins_add = !empty($ins_add) ? $ins_add : $missing;  
  }
  if(isset($content['field_insurance'][0]['node'][$ia_nid]['#node']->body['und'][0]['safe_value']))
    $ins_comm = $content['field_insurance'][0]['node'][$ia_nid]['#node']->body['und'][0]['safe_value'];
  $ins_edit = '<a class="btn btn-primary btn-sm" role="button" target="_blank" href="'.$base_url.'/node/'.$ia_nid.'/edit">edit</a>';
}
  
// Factoring Information
$fa_nid = isset($node->field_factoring['und'][0]['target_id']) ? $node->field_factoring['und'][0]['target_id'] : '';
$fac_com = $missing; $fac_agent = $missing; $fac_email = $missing; $fac_phone = $missing; 
$fac_ext = $missing; $fac_add = $missing;   $fac_comm = $missing;  $fac_edit = $missing; $fac_check = '';
if(!empty($fa_nid)) {
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['#node']->field_cname['und'][0]['value']))
    $fac_agent = $content['field_factoring'][0]['node'][$fa_nid]['#node']->field_cname['und'][0]['value'] . ' ';
  $fac_agent .= $content['field_factoring'][0]['node'][$fa_nid]['#node']->title; 
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['field_factoring_company']['#items'][0]['entity']->title))
    $fac_com = $content['field_factoring'][0]['node'][$fa_nid]['field_factoring_company']['#items'][0]['entity']->title;
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['field_email'][0]['#markup']))
    $fac_email = $content['field_factoring'][0]['node'][$fa_nid]['field_email'][0]['#markup'];
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['field_phone'][0]['#markup']))
    $fac_phone = $content['field_factoring'][0]['node'][$fa_nid]['field_phone'][0]['#markup'];
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['field_ext1'][0]['#markup']))
    $fac_ext = $content['field_factoring'][0]['node'][$fa_nid]['field_ext1'][0]['#markup'];
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['field_address'][0]['#address'])){
    $fadd = $content['field_factoring'][0]['node'][$fa_nid]['field_address'][0]['#address'];
    $fstreet = isset($fadd['thoroughfare']) ? $fadd['thoroughfare'] : '';
    $fcity = isset($fadd['locality']) ? $fadd['locality'] : '';
    $fstate = isset($fadd['administrative_area']) ? $fadd['administrative_area'] : '';
    $fzipc = isset($fadd['postal_code']) ? $fadd['postal_code'] : '';
    $fac_add = $fstreet;
    $fac_add .= !empty($fac_add) && !empty($fcity)  ? ', ' . $fcity : $fcity;  
    $fac_add .= !empty($fac_add) && !empty($fstate) ? ', ' . $fstate : $fstate;  
    $fac_add .= !empty($fzipc) ? ' ' . $fzipc : '';  
    $fac_add = !empty($fac_add) ? $fac_add : $missing;  
  }
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['#node']->body['und'][0]['safe_value']))
    $fac_comm = $content['field_factoring'][0]['node'][$fa_nid]['#node']->body['und'][0]['safe_value'];
  if(isset($content['field_factoring'][0]['node'][$fa_nid]['field_factoring_company']['#items'][0]['entity']->field_web_verification['und'][0]['url'])) {
    $fac_url = $content['field_factoring'][0]['node'][$fa_nid]['field_factoring_company']['#items'][0]['entity']->field_web_verification['und'][0]['url'];
    $fac_check = '<a class="btn btn-primary btn-sm" role="button" target="_blank" href="'.$fac_url.'">check credit</a>';
  }
  $fac_edit = '<a class="btn btn-primary btn-sm" role="button" target="_blank" href="'.$base_url.'/node/'.$fa_nid.'/edit">edit</a>';
}

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ((!$page && !empty($title)) || !empty($title_prefix) || !empty($title_suffix) || $display_submitted): ?>
  <header>
    <?php print render($title_prefix); ?>
    <?php if (!$page && !empty($title)): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($display_submitted): ?>
    <span class="submitted">
      <?php print $user_picture; ?>
      <?php print $submitted; ?>
    </span>
    <?php endif; ?>
  </header>
  <?php endif; ?>
  <?php
    // Hide comments, tags, and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_tags']);
  ?>
  <!-- General Information -->
  <div class="panel panel-success">
    <div class="table-responsive">
      <table class="table">
        <thead>
          <tr class="bg-success text-success">
            <th colspan="3"><span class="glyphicon glyphicon-road" aria-hidden="true"> </span> &nbsp; Carrier Information </th>
            <th colspan="11">
              <div class="progress-bar <?php echo $carrier_color; ?>" role="progressbar" aria-valuemin="0" aria-valuemax="100" 
                   aria-valuenow="<?php echo $carrier_percent; ?>" 
                   style="width:<?php echo $carrier_percent; ?>%"> 
                <?php echo $carrier_percent . ' '; ?> % Complete
              </div>
            </th>
          </tr>
          <tr class="bg-success text-success">
            <th>Operate</th>
            <th>Audited</th>
            <th>Send SMS</th>
            <th>Rate</th>
            <th>MC</th>
            <th>DOT</th>
            <th>TAX ID</th>
            <th>IFTA</th>
            <th>Email</th>
            <th>Phone</th>
            <th>Address</th>
            <th>Comments</th>
            <th>Factoring</th>
            <th>Actions</th>
          </tr>
        </thead>
        <tbody>
          <tr class="bg-success text-success">
            <td><?php echo $operate; ?></td>
            <td><?php echo $audited; ?></td>
            <td><?php echo $notify; ?></td>
            <td><?php echo $rate; ?></td>
            <td><?php echo $mc; ?></td>
            <td><?php echo $dot; ?></td>
            <td><?php echo $taxid; ?></td>
            <td><?php echo $ifta; ?></td>
            <td><?php echo $email; ?> </td>
            <td><?php echo $phone . ' ' . $ext; ?></td>
            <td><?php echo $address; ?></td>
            <td><?php echo $comments; ?></td>
            <td><?php echo $fac_acct . ' ' . $fac_pass; ?></td>
            <td><?php echo $edit . ' ' . $fac_check; ?></td>
          </tr>
        </tbody>
      </table>
    </div> 
  </div>

  <!-- Contact Panel --> 
  <div class="panel panel-info">
    <div class="table-responsive">
      <table class="table">
        <thead>
          <tr class="bg-info text-info">
            <th></th>
            <th>Company</th>
            <th>Agent</th>
            <th>Email</th>
            <th>Phone</th>
            <th>Ext</th>
            <th>Address</th>
            <th>Comments</th>
            <th>Actions</th>
          </tr>
        </thead>
        <tbody>
          <tr class="bg-info text-info">
            <td><span class="glyphicon glyphicon-piggy-bank"></span> &nbsp; Insurance </td>
            <td><?php echo $ins_com; ?></td>
            <td><?php echo $ins_agent; ?></td>
            <td><?php echo $ins_email; ?> </td>
            <td><?php echo $ins_phone; ?></td>
            <td><?php echo $ins_ext; ?></td>
            <td><?php echo $ins_add; ?></td>
            <td><?php echo $ins_comm; ?></td>
            <td><?php echo $ins_edit; ?></td>
          </tr>
          <tr class="bg-info text-info">
            <td><span class="glyphicon glyphicon-cog"></span> &nbsp; Factoring
            <td><?php echo $fac_com; ?></td>
            <td><?php echo $fac_agent; ?></td>
            <td><?php echo $fac_email; ?> </td>
            <td><?php echo $fac_phone; ?></td>
            <td><?php echo $fac_ext; ?></td>
            <td><?php echo $fac_add; ?></td>
            <td><?php echo $fac_comm; ?></td>
            <td><?php echo $fac_edit; ?></td>
          </tr>
        </tbody>
      </table>
    </div> 
  </div>

  <!-- Permits -->
  <div class="panel panel-danger">
    <div class="panel-heading"><span class="glyphicon glyphicon-check" aria-hidden="true"> </span> &nbsp; Permits & Certifications </div>
    <div class="table-responsive">
      <table class="table">
        <thead>
          <tr class="bg-danger text-danger">
            <th>NJ</th>
            <th>NM</th>
            <th>NY</th>
            <th>KYU</th>
            <th>UCR</th>
            <th>IRP</th>
          </tr>
        </thead>
        <tbody>
          <tr class="bg-danger text-danger">
            <td><?php echo $nj; ?></td>
            <td><?php echo $nm; ?></td>
            <td><?php echo $ny; ?></td>
            <td><?php echo $kyu; ?></td>
            <td><?php echo $ucr; ?></td>              
            <td><?php echo $irp; ?></td>
          </tr>          
        </tbody>
      </table>
    </div>       
  </div>

  <!-- Documents Panel -->
  <div class="panel panel-warning">
    <div class="panel-heading"><span class="glyphicon glyphicon-duplicate" aria-hidden="true"> </span> &nbsp; Documents </div>
    <div class="panel-body bg-warning text-warning"> <?php print render($content['field_documetns']); ?> </div>
  </div>
  

  <?php
    // Only display the wrapper div if there are tags or links.
    $field_tags = render($content['field_tags']);
    $links = render($content['links']);
    if ($field_tags || $links):
  ?>
   <footer>
     <?php print $field_tags; ?>
     <?php print $links; ?>
  </footer>
  <?php endif; ?>
  <?php print render($content['comments']); ?>
</article>
