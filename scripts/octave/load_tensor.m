%
% Loads a tensor from file
%
function T = load_tensor(filename)

  % Open the tensor file
  f = fopen("./state.tensor", "rb");

  % Read the rank
  rank = fread(f, 1, "uint32");

  % Figure out the cardinality?
  if rank == 0
    dims = 1;
  else
    dims = fread(f, rank, "uint32");
  end

  % Read the data
  T = fread(f, dims, "float32");

  % Reshape the tensor in the reverse order (for memory coherency), if needed
  if (prod(size(dims)) > 1)
    T	= reshape(T, flipud(dims));

end


